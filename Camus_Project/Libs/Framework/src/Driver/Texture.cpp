#include <Driver/Texture.h>
#include <Utils/FileSystem.h>
#include <Driver/DriverProperties.h>
#include <Utils/Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Driver/stb/stb_image.h>
#include <Driver/cil/cil.h>


namespace hyperspace {
	namespace video {
		Texture			TextureManager::textures[MAX_TEXURE_LIMIT];
		unsigned char	TextureManager::tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
				 char	TextureManager::tex_paths_pool[MAX_TEXURE_LIMIT][512];

		unsigned int	TextureManager::num_textures_loaded = 0;
		unsigned int	TextureManager::current_index = 0;

		TextureManager::TextureManager(){
		
		}

		unsigned short	TextureManager::LoadTexture(std::string filename, unsigned int params) {
#if USE_LOG_DEBUG_TEX_LOADING
			LogPrintDebug("[LoadTexture] Loading texture [%s]", filename.c_str());
#endif

			unsigned short ret = TEXTURE_NOT_LOADED;

			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Textures/";
			Path += filename;

			std::ifstream in_(Path.c_str(), std::ios::binary | std::ios::in);
			if (!in_.good()) {
				in_.close();
				LogPrintError("[LoadTexture] %s not found \n\n", Path.c_str());
				return ret;
			}

			unsigned int format = CheckFormat(in_);

			in_.close();

			if (format == 0) {
				LogPrintError("[LoadTexture] %s cannot determine format.\n\n", Path.c_str());
				return ret;
			}

			if (format & (file_format::BMP | file_format::PNG | file_format::TGA)) {
				ret = LoadBufferUncompressed(Path, format, params);
			}else if (format & (file_format::PVR | file_format::DDS | file_format::KTX)) {
				ret = LoadBufferCompressed(Path, format,params);
			}
						

			return ret;
		}

		unsigned int TextureManager::LoadBufferUncompressed(std::string &Path, unsigned int format, unsigned int params) {
#if USE_LOG_DEBUG_TEX_LOADING
			LogPrintDebug("[LoadTexture] Loading Uncompressed texture [%s]", Path.c_str());
#endif
			int index = 0;
			int offset = 0;
			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (!textures[i].bounded) {
					index = i;
					break;
				}
				else {
					offset += textures[i].size;
				}
			}

			Texture *tex = &textures[index];
			int x = 0, y = 0, channels = 0;
			unsigned char *buffer = stbi_load(Path.c_str(), &x, &y, &channels, 0);

			if (buffer == 0) {
				LogPrintError("[LoadTexture] Failed at loading texture [%s] \n\n", Path.c_str());
				return TEXTURE_NOT_LOADED;
			}

			tex->size = x*y*channels;

			unsigned int usage_mem = offset + tex->size;
			if (usage_mem >= TEXTURE_BUDGET_SIZE_BYTES)
				LogPrintWarning("[LoadTexture] Current Ram Pool Usage: %d of total: %d. Textures may be corrupted", usage_mem, TEXTURE_BUDGET_SIZE_BYTES);

			memcpy((void*)(tex_mem_pool + offset), (void*)&buffer[0], tex->size);

			stbi_image_free(buffer);

			if (Path.size() > 32) {
				std::string sub = Path.substr(0, 511);
#ifdef OS_ANDROID
				strcpy(tex_paths_pool[index], sub.c_str());
#else
				strcpy_s(tex_paths_pool[index], sub.c_str());
#endif
				tex_paths_pool[index][511] = '\0';
			}
			else {
#ifdef OS_ANDROID
				strcpy(tex_paths_pool[index], Path.c_str());
#else
				strcpy_s(tex_paths_pool[index], Path.c_str());
#endif
				tex_paths_pool[index][Path.size()] = '\0';
			}

			tex->bounded = 1;
			tex->x = x;
			tex->y = y;
			tex->mipmaps = 1;
			tex->offset = offset;
			tex->props |= format;
			tex->props |= compress_format::RAW;
			tex->props |= pixel_format_::PFMT_UNSIGNED;
			tex->props |= pixel_format_::PFMT_BYTE;
			tex->props |= bpp_::BPP_8;

			switch (channels) {
			case 1: {
				tex->props |= channelS_::CH_ALPHA;
			}break;
			case 3: {
				tex->props |= channelS_::CH_RGB;
			}break;
			case 4: {
				tex->props |= channelS_::CH_RGBA;
			}break;
			}
			
			LoadAPITexture(tex, tex_mem_pool + offset, params);
	
			current_index = index;
			num_textures_loaded++;
#if USE_LOG_DEBUG_TEX_LOADING
			PrintTextureInfo(Path, tex);
#else
			LogPrintDebug("[LoadTexture] Loaded [%s][%dx%dx%d][%dkb]\n\n", tex_paths_pool[index], x, y, channels*8, tex->size / 1024);
#endif

			return TEXTURE_LOADED;
		}

		unsigned int TextureManager::LoadBufferCompressed(std::string &Path, unsigned int format, unsigned int params) {
#if USE_LOG_DEBUG_TEX_LOADING
			LogPrintDebug("[LoadTexture] Loading Compressed texture [%s]", Path.c_str());
#endif
			int index = 0;
			int offset = 0;
			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (!textures[i].bounded) {
					index = i;
					break;
				}
				else {
					offset += textures[i].size;
				}
			}
			
			int x = 0, y = 0;
			unsigned int props = 0;
			unsigned int buffer_size = 0;
			unsigned char mipmaps = 0;
			unsigned char *buffer = cil_load(Path.c_str(), &x, &y, &mipmaps, &props, &buffer_size);

			bool ext_good = CheckIfExtensionIsSupported(Path,props);

			if (!ext_good) {
				cil_free_buffer(buffer);
				return TEXTURE_NOT_LOADED;
			}

			if (buffer == 0) {
				switch (props) {
				case CIL_NOT_FOUND: {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] Not Found. \n\n", Path.c_str());
				}break;
				case CIL_CORRUPT: {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] Is Corrupt.\n\n ", Path.c_str());
				}break;
				case CIL_NO_MEMORY: {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] Not enough memory.\n\n ", Path.c_str());
				}break;
				case CIL_PVR_V2_NOT_SUPPORTED: {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] Pvr v2 not supported.\n\n ", Path.c_str());
				}break;
				case CIL_NOT_SUPPORTED_FILE: {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] Not supported format.\n\n ", Path.c_str());
				}break;
				}
			}

			unsigned int usage_mem = offset + buffer_size;
			if (usage_mem >= TEXTURE_BUDGET_SIZE_BYTES)
				LogPrintWarning("[LoadTexture] Current Ram Pool Usage: %d of total: %d. Textures may be corrupted", usage_mem, TEXTURE_BUDGET_SIZE_BYTES);

			memcpy((void*)(tex_mem_pool + offset), (void*)&buffer[0], buffer_size);

			cil_free_buffer(buffer);

			if (Path.size() > 32) {
				std::string sub = Path.substr(0, 511);
#ifdef OS_ANDROID
				strcpy(tex_paths_pool[index], sub.c_str());
#else
				strcpy_s(tex_paths_pool[index], sub.c_str());
#endif
				tex_paths_pool[index][511] = '\0';
			}
			else {
#ifdef OS_ANDROID
				strcpy(tex_paths_pool[index], Path.c_str());
#else
				strcpy_s(tex_paths_pool[index], Path.c_str());
#endif
				tex_paths_pool[index][Path.size()] = '\0';
			}

			Texture *tex = &textures[index];
			tex->size = buffer_size;
			tex->props = props;
			tex->mipmaps = mipmaps;
			tex->x = x;	tex->y = y;
			tex->bounded = 1;

			LoadAPITextureCompressed(tex, (tex_mem_pool + offset), params);

			unsigned int bpp = 0;
			if (tex->props & bpp_::BPP_2)
				bpp = 2;
			if (tex->props & bpp_::BPP_4)
				bpp = 4;
			if (tex->props & bpp_::BPP_8)
				bpp = 8;

			current_index = index;
			num_textures_loaded++;

#if USE_LOG_DEBUG_TEX_LOADING
			PrintTextureInfo(Path, tex);
#else
			LogPrintDebug("[LoadTexture] Loaded [%s][%dx%d][%dbpp][%dkb]\n\n", tex_paths_pool[index], x, y, bpp, tex->size / 1024);
#endif

			return TEXTURE_LOADED;
		}

#if USE_LOG_DEBUG_TEX_LOADING
		void	TextureManager::PrintTextureInfo(std::string &name, Texture *tex) {
			unsigned int mipmaps_count = tex->mipmaps;
			unsigned int props = tex->props;
			unsigned int bpp = 8;
			unsigned int size = tex->size;

			std::string rgba_;
			std::string compress_ = "RAW";

			if (props & channelS_::CH_RGB)
				rgba_ = "RGB";
			if (props & channelS_::CH_RGBA)
				rgba_ = "RGB";
			if (props & channelS_::CH_ALPHA)
				rgba_ = "ALPHA";

			if (props & compress_format::PVRTC2) {
				compress_ = "PVRTC2";
				bpp = 2;
			}

			if (props & compress_format::PVRTC4) {
				compress_ = "PVRTC4";
				bpp = 4;
			}

			if (props & compress_format::PVRTCII2) {
				compress_ = "PVRTCII2";
				bpp = 2;
			}

			if (props & compress_format::PVRTCII4) {
				compress_ = "PVRTCII4";
				bpp = 4;
			}

			if (props & compress_format::ETC1) {
				compress_ = "ETC1";
				bpp = 4;
			}
			LogPrintDebug("[LoadTexture] Loaded:\n\n[Name][%s]\n[Compression][%s]\n[Channels][%s]\n[Size][%d]\n[Dimensions][%dx%d]\n[BPP][%d]\n[Mipmaps][%d]\n\n",
				name.c_str(),
				compress_.c_str(),
				rgba_.c_str(),
				size,
				tex->x,
				tex->y,
				bpp,
				mipmaps_count
				);

		}
#endif

		unsigned int TextureManager::CheckFormat(std::ifstream &in_) {

			std::streampos begPos = in_.tellg();

			//	Raw formats:
			{
				unsigned char signaturePNG[8];
				in_.read((char*)signaturePNG, 8);
				int number = 0;
				for (int i = 0; i < 8; i++) {
					number += (int)signaturePNG[i];
				}

				if (number == 425) {
					return file_format::PNG;
				}
			}

			{
				in_.seekg(begPos);
				unsigned char pad[12];
				short x, y;
				char bpp;
				in_.read((char*)pad, 12);
				in_.read((char*)&x, 2);
				in_.read((char*)&y, 2);
				in_.read((char*)&bpp, 1);

				if (x > 0 && y > 0 && bpp > 0 && x < 10000 && y < 10000 && bpp < 50) {
					return file_format::TGA;
				}
			}

			{
				in_.seekg(begPos);
				char	dds[4];
				in_.read((char*)dds, 3);
				dds[3] = '\0';
				if (strcmp(dds, "DDS") == 0) {
					return file_format::DDS;
				}
			}

			{
				in_.seekg(begPos);
				char	bmp[3];
				in_.read((char*)bmp, 2);
				bmp[2] = '\0';
				if (strcmp(bmp, "BM") == 0) {
					return file_format::BMP;
				}
			}

			// Compressed formats:
			{
				in_.seekg(begPos);
				unsigned char	ktx[5];
				in_.read((char*)ktx, 4);
				ktx[0] = 0;
				ktx[4] = '\0';
				if (strcmp((char*)ktx, " KTX") == 0) {
					return file_format::KTX;
				}
			}

			{
				in_.seekg(begPos);
				char pvr[4];
				in_.read((char*)&pvr, 3);
				pvr[3] = '\0';
				if (strcmp((char*)pvr, "PVR") == 0) {
					return file_format::PVR;
				}
			}

			return 0;
		}

		bool TextureManager::CheckIfExtensionIsSupported(std::string &name, unsigned int &props) {


			if (props & (compress_format::PVRTC2 | compress_format::PVRTC4 | compress_format::PVRTCII2 | compress_format::PVRTCII4)) {
				if (GetDriverProperties().isExtensionSupported("GL_IMG_texture_compression_pvrtc")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension GL_IMG_texture_compression_pvrtc not supported. \n\n", name.c_str());
					return false;
				}
			}

			if (props & compress_format::ETC1) {
				if (GetDriverProperties().isExtensionSupported("GL_OES_compressed_ETC1_RGB8_texture")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension GL_IMG_texture_compression_pvrtc not supported. \n\n", name.c_str());
					return false;
				}
			}

			if (props & compress_format::ETC2) {
				if (GetDriverProperties().isExtensionSupported("OES_compressed_ETC2_RGBA8_texture")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension OES_compressed_ETC2_RGBA8_texture not supported. \n\n", name.c_str());
					return false;
				}
			}

			if (props & compress_format::DXT1) {
				if (GetDriverProperties().isExtensionSupported("GL_EXT_texture_compression_dxt1")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension GL_EXT_texture_compression_dxt1 not supported. \n\n", name.c_str());
					return false;
				}
			}

			
			LogPrintError("[LoadTexture] Failed at loading texture [%s] extension Unknown and not supported. \n\n", name.c_str());
			return false;
		}
		
		void TextureManager::LoadAPITexture(Texture *tex, unsigned char* buffer, unsigned int &params) {
		
		}

		void TextureManager::LoadAPITextureCompressed(Texture *tex, unsigned char* buffer, unsigned int &params) {
	
		}
	}
}