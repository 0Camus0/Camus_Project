#include <config.h>
#include <Driver\TextureManager.h>
#include <Utils\FileSystem.h>
#include <Driver\DriverProperties.h>
#include <Utils\Log.h>

#if  defined(USING_OPENGL_ES)
#include <Driver\TextureGL.h>
#elif defined(USING_D3D11)
#include <Driver\TextureD3D11.h>
#endif


#define STB_IMAGE_IMPLEMENTATION
#include <Driver\stb/stb_image.h>
#include <Driver\cil/cil.h>

#if defined(OS_ANDROID) || defined(__APPLE__)
#define strcpy_s strcpy
#endif

namespace hyperspace {
	namespace video {
		unsigned int	TextureManager::queue[MAX_TEXURE_LIMIT];
		unsigned int	TextureManager::queueoffsets[MAX_TEXURE_LIMIT];
		Texture*		TextureManager::textures[MAX_TEXURE_LIMIT];
		unsigned char	TextureManager::tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
		char			TextureManager::tex_paths_pool[MAX_TEXURE_LIMIT][512];

		unsigned int	TextureManager::num_textures_loaded = 0;
		unsigned int	TextureManager::current_index = 0;

		TextureManager::TextureManager() {
			memset(queue, 0, MAX_TEXURE_LIMIT * sizeof(unsigned int));
			memset(queueoffsets, 0, MAX_TEXURE_LIMIT * sizeof(unsigned int));
			memset(tex_mem_pool, 0, TEXTURE_BUDGET_SIZE_BYTES);
			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				textures[i] = 0;
			}
		}



		Texture*		TextureManager::LoadTexture(std::string filename, unsigned int params) {
#if USE_LOG_DEBUG_TEX_LOADING
			LogPrintDebug("[LoadTexture] Loading texture [%s]", filename.c_str());
#endif
			Texture *ret = 0;

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
			}
			else if (format & (file_format::PVR | file_format::DDS | file_format::KTX)) {
				ret = LoadBufferCompressed(Path, format, params);
			}


			return ret;
		}

		Texture* TextureManager::LoadBufferUncompressed(std::string &Path, unsigned int format, unsigned int params) {
#if USE_LOG_DEBUG_TEX_LOADING
			LogPrintDebug("[LoadBufferUncompressed] Loading Uncompressed texture [%s]", Path.c_str());
#endif


			Texture *texture = 0;

			int x = 0, y = 0, channels = 0;
			unsigned char *buffer = stbi_load(Path.c_str(), &x, &y, &channels, 0);

			if (buffer == 0) {
				LogPrintError("[LoadBufferUncompressed] Failed at loading texture [%s] \n\n", Path.c_str());
				return texture;
			}

#if  defined(USING_OPENGL_ES)
			texture = new TextureGL;
#elif defined(USING_D3D11)

#endif

			texture->size = x*y*channels;
			texture->bounded = 1;
			texture->x = x;
			texture->y = y;
			texture->mipmaps = 1;
			texture->params = params;
			texture->props |= format;
			texture->props |= compress_format::RAW;
			texture->props |= pixel_format_::PFMT_UNSIGNED;
			texture->props |= pixel_format_::PFMT_BYTE;
			texture->props |= bpp_::BPP_8;

			switch (channels) {
			case 1: {
				texture->props |= channelS_::CH_ALPHA;
			}break;
			case 3: {
				texture->props |= channelS_::CH_RGB;
			}break;
			case 4: {
				texture->props |= channelS_::CH_RGBA;
			}break;
			}


			memcpy(&texture->optname[0], Path.c_str(), Path.size());
			texture->optname[Path.size()] = '\0';

			texture->LoadAPITexture(buffer);
			stbi_image_free(buffer);

#if USE_LOG_DEBUG_TEX_LOADING
			PrintTextureInfo(Path, texture);
#else
			LogPrintDebug("[LoadBufferUncompressed] Loaded [%s][%dx%dx%d][%dkb]\n\n", Path.c_str(), x, y, channels * 8, texture->size / 1024);
#endif

			return texture;
		}

		Texture* TextureManager::LoadBufferCompressed(std::string &Path, unsigned int format, unsigned int params) {
#if USE_LOG_DEBUG_TEX_LOADING
			LogPrintDebug("[LoadBufferCompressed] Loading Compressed texture [%s]", Path.c_str());
#endif

			Texture *texture = 0;	

			int x = 0, y = 0;
			unsigned int props = 0;
			unsigned int buffer_size = 0;
			unsigned char mipmaps = 0;
			unsigned char *buffer = cil_load(Path.c_str(), &x, &y, &mipmaps, &props, &buffer_size);

			bool ext_good = CheckIfExtensionIsSupported((char*)Path.c_str(), props);

			if (!ext_good) {
				cil_free_buffer(buffer);
				return texture;
			}

			if (buffer == 0) {
				switch (props) {
					case CIL_NOT_FOUND: {
						LogPrintError("[LoadBufferCompressed] Failed at loading texture [%s] Not Found. \n\n", Path.c_str());
					}break;
					case CIL_CORRUPT: {
						LogPrintError("[LoadBufferCompressed] Failed at loading texture [%s] Is Corrupt.\n\n ", Path.c_str());
					}break;
					case CIL_NO_MEMORY: {
						LogPrintError("[LoadBufferCompressed] Failed at loading texture [%s] Not enough memory.\n\n ", Path.c_str());
					}break;
					case CIL_PVR_V2_NOT_SUPPORTED: {
						LogPrintError("[LoadBufferCompressed] Failed at loading texture [%s] Pvr v2 not supported.\n\n ", Path.c_str());
					}break;
					case CIL_NOT_SUPPORTED_FILE: {
						LogPrintError("[LoadBufferCompressed] Failed at loading texture [%s] Not supported format.\n\n ", Path.c_str());
					}break;
				}
				return texture;
			}

#if  defined(USING_OPENGL_ES)
			texture = new TextureGL;
#elif defined(USING_D3D11)

#endif
			texture->bounded = 1;
			texture->size = buffer_size;
			texture->props = props;
			texture->mipmaps = mipmaps;
			texture->x = x;	texture->y = y;
			texture->params = params;
			texture->props |= props_::COMPRESSED;

			memcpy(&texture->optname[0], Path.c_str(), Path.size());
			texture->optname[Path.size()] = '\0';


			texture->LoadAPITextureCompressed(buffer);

			cil_free_buffer(buffer);

			unsigned int bpp = 0;
			if (texture->props & bpp_::BPP_2)
				bpp = 2;
			if (texture->props & bpp_::BPP_4)
				bpp = 4;
			if (texture->props & bpp_::BPP_8)
				bpp = 8;

#if USE_LOG_DEBUG_TEX_LOADING
			PrintTextureInfo(Path, texture);
#else
			LogPrintDebug("[LoadBufferCompressed] Loaded [%s][%dx%d][%dbpp][%dkb]\n\n", Path.c_str(), x, y, bpp, texture->size / 1024);
#endif

			return texture;
		}

		unsigned int TextureManager::AddTextureToLoadingQueue(std::string filename, unsigned int params) {
#if USE_LOG_DEBUG_TEX_LOADING
			LogPrintDebug("[AddTextureToLoadingQueue] Adding texture [%s] to the queue", filename.c_str());
#endif

			unsigned int ret = TEXTURE_BAD_INDEX;

			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Textures/";
			Path += filename;

			std::ifstream in_(Path.c_str(), std::ios::binary | std::ios::in);
			if (!in_.good()) {
				in_.close();
				LogPrintError("[LoadTextureQueue] %s not found \n\n", Path.c_str());
				return ret;
			}
			unsigned int format = CheckFormat(in_);
			in_.close();

			if (format == 0) {
				LogPrintError("[LoadTextureQueue] %s cannot determine format.\n\n", Path.c_str());
				return ret;
			}

			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (queue[i] == 0) {
					current_index = i;
					num_textures_loaded = current_index + 1;
					queue[current_index] = format | params;
					strcpy_s(tex_paths_pool[current_index], Path.c_str());
					tex_paths_pool[current_index][Path.size()] = '\0';
					if (format & (file_format::PVR | file_format::DDS | file_format::KTX)) {
						queue[current_index] |= (format & (file_format::PVR | file_format::DDS | file_format::KTX)) ? props_::COMPRESSED : 0;
					}

					return current_index;
					break;
				}
			}


			LogPrintError("[LoadTextureQueue] MAX_TEXURE_LIMIT %d exceeded, %s not added to the queue.\n\n", MAX_TEXURE_LIMIT, Path.c_str());
			return ret;

		}

		unsigned int TextureManager::LoadBufferUncompressedQueue(unsigned int &index) {
			unsigned int ret = TEXTURE_NOT_LOADED;
			char path[MAX_PATH_SIZE];

			strcpy_s(path, tex_paths_pool[index]);

			int x = 0, y = 0, channels = 0;
			unsigned char *buffer = stbi_load(path, &x, &y, &channels, 0);

			if (buffer == 0) {
				LogPrintError("[LoadBufferUncompressedQueue] Failed at loading texture [%s] \n\n", path);
				return ret;
			}

			unsigned int size_ = x*y*channels;
			unsigned int offset = 0;
			for (unsigned int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (queueoffsets[i] == 0)
					break;
				else
					offset += queueoffsets[i];
			}
			queueoffsets[index] = size_;

			unsigned int usage_mem = offset + size_;
			if (usage_mem >= TEXTURE_BUDGET_SIZE_BYTES)
				LogPrintWarning("[LoadBufferUncompressedQueue] Current Ram Pool Usage: %d of total: %d. Textures may be corrupted", usage_mem, TEXTURE_BUDGET_SIZE_BYTES);

			memcpy((void*)(tex_mem_pool + offset), (void*)&buffer[0], size_);

			stbi_image_free(buffer);
			unsigned int format = queue[index] & 0x3F0000;
			unsigned int params = queue[index] & 0x3FF;
#if  defined(USING_OPENGL_ES)
			Texture *texture = new TextureGL;
#elif defined(USING_D3D11)
			Texture *texture = new TextureD3D11;
#endif
			texture->bounded = 1;
			texture->x = x;
			texture->y = y;
			texture->size = size_;
			texture->mipmaps = 1;
			texture->params = params;
			texture->props |= format;
			texture->props |= compress_format::RAW;
			texture->props |= pixel_format_::PFMT_UNSIGNED;
			texture->props |= pixel_format_::PFMT_BYTE;
			texture->props |= bpp_::BPP_8;

			switch (channels) {
			case 1: {
				texture->props |= channelS_::CH_ALPHA;
			}break;
			case 3: {
				texture->props |= channelS_::CH_RGB;
			}break;
			case 4: {
				texture->props |= channelS_::CH_RGBA;
			}break;
			}

#if USE_LOG_DEBUG_TEX_LOADING
			PrintTextureInfo(path, texture);
#else
			LogPrintDebug("[LoadBufferUncompressedQueue] Loaded [%s][%dx%dx%d][%dkb]\n\n", path, x, y, channels * 8, size_ / 1024);
#endif
			textures[index] = texture;

			return TEXTURE_LOADED;
		}

		unsigned int TextureManager::LoadBufferCompressedQueue(unsigned int &index) {
			unsigned int ret = TEXTURE_NOT_LOADED;
			char path[MAX_PATH_SIZE];

			strcpy_s(path, tex_paths_pool[index]);

			int x = 0, y = 0;
			unsigned int props = 0;
			unsigned int buffer_size = 0;
			unsigned char mipmaps = 0;
			unsigned char *buffer = cil_load(path, &x, &y, &mipmaps, &props, &buffer_size);

			if (buffer == 0) {
				LogPrintError("[LoadBufferCompressedQueue] Failed at loading texture [%s] \n\n", path);
				return ret;
			}

			bool ext_good = CheckIfExtensionIsSupported(path, props);

			if (!ext_good) {
				cil_free_buffer(buffer);
				return TEXTURE_NOT_LOADED;
			}

			if (buffer == 0) {
				switch (props) {
				case CIL_NOT_FOUND: {
					LogPrintError("[LoadBufferCompressedQueue] Failed at loading texture [%s] Not Found. \n\n", path);
				}break;
				case CIL_CORRUPT: {
					LogPrintError("[LoadBufferCompressedQueue] Failed at loading texture [%s] Is Corrupt.\n\n ", path);
				}break;
				case CIL_NO_MEMORY: {
					LogPrintError("[LoadBufferCompressedQueue] Failed at loading texture [%s] Not enough memory.\n\n ", path);
				}break;
				case CIL_PVR_V2_NOT_SUPPORTED: {
					LogPrintError("[LoadBufferCompressedQueue] Failed at loading texture [%s] Pvr v2 not supported.\n\n ", path);
				}break;
				case CIL_NOT_SUPPORTED_FILE: {
					LogPrintError("[LoadBufferCompressedQueue] Failed at loading texture [%s] Not supported format.\n\n ", path);
				}break;
				}
			}

			unsigned int offset = 0;
			for (unsigned int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (queueoffsets[i] == 0)
					break;
				else
					offset += queueoffsets[i];
			}
			queueoffsets[index] = buffer_size;

			unsigned int usage_mem = offset + buffer_size;
			if (usage_mem >= TEXTURE_BUDGET_SIZE_BYTES)
				LogPrintWarning("[LoadBufferCompressedQueue] Current Ram Pool Usage: %d of total: %d. Textures may be corrupted", usage_mem, TEXTURE_BUDGET_SIZE_BYTES);

			memcpy((void*)(tex_mem_pool + offset), (void*)&buffer[0], buffer_size);

			cil_free_buffer(buffer);

			unsigned int params = queue[index] & 0x3FF;
#if  defined(USING_OPENGL_ES)
			Texture *texture = new TextureGL;
#elif defined(USING_D3D11)
			Texture *texture = new TextureD3D11;
#endif
			texture->size = buffer_size;
			texture->props = props;
			texture->mipmaps = mipmaps;
			texture->x = x;	texture->y = y;
			texture->bounded = 1;
			texture->params = params;
			texture->props |= props_::COMPRESSED;

			unsigned int bpp = 0;
			if (texture->props & bpp_::BPP_2)
				bpp = 2;
			if (texture->props& bpp_::BPP_4)
				bpp = 4;
			if (texture->props & bpp_::BPP_8)
				bpp = 8;

#if USE_LOG_DEBUG_TEX_LOADING
			PrintTextureInfo(Path, tex);
#else
			LogPrintDebug("[LoadTexture] Loaded [%s][%dx%d][%dbpp][%dkb]\n\n", path, x, y, bpp, buffer_size / 1024);
#endif
			textures[index] = texture;

			return TEXTURE_LOADED;
		}

		void TextureManager::LoadTextureQueue() {

			for (unsigned int i = 0; i < num_textures_loaded; i++) {
				if (queue[i] & props_::COMPRESSED) {
					LoadBufferCompressedQueue(i);
				}
				else {
					LoadBufferUncompressedQueue(i);
				}
			}

			int offset = 0;
			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (textures[i]==0) {
					break;
				}
				else {
					#if  defined(USING_OPENGL_ES)
						TextureGL *texture = dynamic_cast<TextureGL*>(textures[i]);
					#elif defined(USING_D3D11)
						TextureD3D11 *texture = dynamic_cast<TextureD3D11*>(textures[i]);
					#endif
					if (texture->props&props_::COMPRESSED) {
						texture->LoadAPITextureCompressed((tex_mem_pool + offset));
					}
					else {
						texture->LoadAPITexture(tex_mem_pool + offset);
					}
					offset += textures[i]->size;
				}
			}

		}

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
				ktx[0] = ' ';
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

		bool TextureManager::CheckIfExtensionIsSupported(char *name, unsigned int &props) {


			if (props & (compress_format::PVRTC2 | compress_format::PVRTC4 | compress_format::PVRTCII2 | compress_format::PVRTCII4)) {
				if (GetDriverProperties().isExtensionSupported("GL_IMG_texture_compression_pvrtc")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension GL_IMG_texture_compression_pvrtc not supported. \n\n", name);
					return false;
				}
			}

			if (props & compress_format::ETC1) {
				if (GetDriverProperties().isExtensionSupported("GL_OES_compressed_ETC1_RGB8_texture")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension GL_IMG_texture_compression_pvrtc not supported. \n\n", name);
					return false;
				}
			}

			if (props & compress_format::ETC2) {
				if (GetDriverProperties().isExtensionSupported("OES_compressed_ETC2_RGBA8_texture")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension OES_compressed_ETC2_RGBA8_texture not supported. \n\n", name);
					return false;
				}
			}

			if (props & compress_format::DXT1) {
				if (GetDriverProperties().isExtensionSupported("GL_EXT_texture_compression_dxt1")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension GL_EXT_texture_compression_dxt1 not supported. \n\n", name);
					return false;
				}
			}

			if (props & compress_format::DXT5) {
				if (GetDriverProperties().isExtensionSupported("GL_EXT_texture_compression_s3tc")) {
					return true;
				}
				else {
					LogPrintError("[LoadTexture] Failed at loading texture [%s] extension GL_EXT_texture_compression_s3tc not supported. \n\n", name);
					return false;
				}
			}

			LogPrintError("[LoadTexture] Failed at loading texture [%s] extension Unknown and not supported. \n\n", name);
			return false;
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
	}
}