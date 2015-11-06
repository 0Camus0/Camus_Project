#include <Driver/Texture.h>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Driver/stb/stb_image.h>
#include <Driver/cil/cil.h>

#include <Driver/OpenGLDriver.h>

namespace hyperspace {
	namespace video {
		Texture			TextureManager::textures[MAX_TEXURE_LIMIT];
		unsigned char	TextureManager::tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
				 char	TextureManager::tex_paths_pool[MAX_TEXURE_LIMIT][512];

		unsigned int	TextureManager::num_textures_loaded = 0;
		unsigned int	TextureManager::current_index = 0;

		TextureManager::TextureManager(){
		
		}

		unsigned short	TextureManager::LoadTexture(std::string filename) {
			LogPrintDebug("[TextureManager::LoadTexture] %s", filename.c_str());

			unsigned short ret = TEXTURE_NOT_FOUND;

			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Textures/";
			Path += filename;

			std::ifstream in_(Path.c_str(), std::ios::binary | std::ios::in);
			if (!in_.good()) {
				in_.close();
				LogPrintError("[TextureManager::LoadTexture] %s not found \n\n", Path.c_str());
				return ret;
			}

			unsigned int format = CheckFormat(in_);

			in_.close();

			if (format == 0) {
				LogPrintError("[TextureManager::LoadTexture] %s cannot determine format.\n\n", Path.c_str());
				return ret;
			}

			if (format & (file_format::BMP | file_format::PNG | file_format::TGA)) {
				ret = LoadBufferUncompressed(Path, format);
			}else if (format & (file_format::PVR | file_format::DDS | file_format::KTX)) {
				ret = LoadBufferCompressed(Path, format);
			}
						

			return ret;
		}

		unsigned int TextureManager::LoadBufferUncompressed(std::string &Path, unsigned int format) {
			int id = 0;
			int offset = 0;
			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (!textures[i].bounded) {
					id = i;
					break;
				}
				else {
					offset += textures[i].size;
				}
			}

			Texture *tex = &textures[id];
			int x = 0, y = 0, channels = 0;
			unsigned char *buffer = stbi_load(Path.c_str(), &x, &y, &channels, 0);

			if (buffer == 0) {
				LogPrintError("[TextureManager::LoadTexture] Failed at loading texture [%s] \n\n", Path.c_str());
				return TEXTURE_NOT_FOUND;
			}

			

			tex->size = x*y*channels;
			memcpy((void*)(tex_mem_pool + offset), (void*)&buffer[0], tex->size);
			stbi_image_free(buffer);

			if (Path.size() > 32) {
				std::string sub = Path.substr(0, 511);
#ifdef OS_ANDROID
				strcpy(tex_paths_pool[id], sub.c_str());
#else
				strcpy_s(tex_paths_pool[id], sub.c_str());
#endif
				tex_paths_pool[id][511] = '\0';
			}
			else {
#ifdef OS_ANDROID
				strcpy(tex_paths_pool[id], Path.c_str());
#else
				strcpy_s(tex_paths_pool[id], Path.c_str());
#endif
				tex_paths_pool[id][Path.size()] = '\0';
			}

			tex->bounded = 1;
			tex->id = id;
			tex->x = x;
			tex->y = y;
			tex->mipmaps = 1;
			tex->offset = offset;
			tex->props |= format;
			tex->props |= compress_format::RAW;
			tex->props |= pixel_format_::UINTEGER_8;
			tex->props |= bpp_::BPP_8;
						
			
			
			unsigned int id__;
			glGenTextures(1, &id__);
			glBindTexture(GL_TEXTURE_2D, id__);
			if (channels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)(tex_mem_pool + offset));
			else if (channels == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(tex_mem_pool + offset));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			tex->id = static_cast<unsigned short>(id__);


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

			current_index = id;
			num_textures_loaded++;



			LogPrintDebug("[TextureManager::LoadTexture] Loaded [%s] info: [%d x %d x %d]  %d kb. \n\n", tex_paths_pool[id], x, y, channels*8, tex->size / 1024);

			return TEXTURE_FOUND;
		}

		unsigned int TextureManager::LoadBufferCompressed(std::string &Path, unsigned int format){

			int x = 0, y = 0;
			unsigned int props = 0;
			unsigned char *buffer = cil_load(Path.c_str(),&x,&y,&props);

			if (buffer == 0) {
				switch (props){
					case CIL_NOT_FOUND: {
						LogPrintError("[TextureManager::LoadTexture] Failed at loading texture [%s] Not Found. \n\n", Path.c_str());
					}break;
					case CIL_CORRUPT: {
						LogPrintError("[TextureManager::LoadTexture] Failed at loading texture [%s] Is Corrupt.\n\n ", Path.c_str());
					}break;
					case CIL_NO_MEMORY: {
						LogPrintError("[TextureManager::LoadTexture] Failed at loading texture [%s] Not enough memory.\n\n ", Path.c_str());
					}break;
					case CIL_PVR_V2_NOT_SUPPORTED: {
						LogPrintError("[TextureManager::LoadTexture] Failed at loading texture [%s] Pvr v2 not supported.\n\n ", Path.c_str());
					}break;
					case CIL_NOT_SUPPORTED_FILE: {
						LogPrintError("[TextureManager::LoadTexture] Failed at loading texture [%s] Not supported format.\n\n ", Path.c_str());
					}break;				
				}
			}

			return TEXTURE_FOUND;
		}

		unsigned int TextureManager::CheckFormat(std::ifstream &in_) {

			std::streampos begPos = in_.tellg();

			//	Raw formats:
			{
				LogPrintDebug("[TextureManager::LoadTexture] Checking if is png ");
				unsigned char signaturePNG[8];
				in_.read((char*)signaturePNG, 8);
				int number = 0;
				for (int i = 0; i < 8; i++) {
					number += (int)signaturePNG[i];
				}

				if (number == 425) {
					LogPrintDebug("[TextureManager::LoadTexture] Is PNG");
					return file_format::PNG;
				}
				else {
					LogPrintDebug("[TextureManager::LoadTexture] Is not PNG");
				}
			}

			{
				in_.seekg(begPos);
				LogPrintDebug("[TextureManager::LoadTexture] Checking if is tga ");
				unsigned char pad[12];
				short x, y;
				char bpp;
				in_.read((char*)pad, 12);
				in_.read((char*)&x, 2);
				in_.read((char*)&y, 2);
				in_.read((char*)&bpp, 1);

				if (x > 0 && y > 0 && bpp > 0 && x < 10000 && y < 10000 && bpp < 50) {
					LogPrintDebug("[TextureManager::LoadTexture] Is TGA");
					return file_format::TGA;
				}
				else {
					LogPrintDebug("[TextureManager::LoadTexture] Is not PNG");
				}
			}

			{
				in_.seekg(begPos);
				LogPrintDebug("[TextureManager::LoadTexture] Checking if is DDS ");
				char	dds[4];
				in_.read((char*)dds, 3);
				dds[3] = '\0';
				if (strcmp(dds, "DDS") == 0) {
					LogPrintDebug("[TextureManager::LoadTexture] Is DDS");
					return file_format::DDS;
				}
				else {
					LogPrintDebug("[TextureManager::LoadTexture] Is not DDS");
				}
			}

			{
				in_.seekg(begPos);
				LogPrintDebug("[TextureManager::LoadTexture] Checking if is BMP ");
				char	bmp[3];
				in_.read((char*)bmp, 2);
				bmp[2] = '\0';
				if (strcmp(bmp, "BM") == 0) {
					LogPrintDebug("[TextureManager::LoadTexture] Is BMP");
					return file_format::BMP;
				}
				else {
					LogPrintDebug("[TextureManager::LoadTexture] Is not BMP");
				}
			}

			// Compressed formats:
			{
				in_.seekg(begPos);
				LogPrintDebug("[TextureManager::LoadTexture] Checking if is KTX ");
				unsigned char	ktx[5];
				in_.read((char*)ktx, 4);
				ktx[4] = '\0';
				if (strcmp((char*)ktx, "�KTX") == 0) {
					LogPrintDebug("[TextureManager::LoadTexture] Is KTX");
					return file_format::KTX;
				}
				else {
					LogPrintDebug("[TextureManager::LoadTexture] Is not KTX");
				}
			}

			{
				in_.seekg(begPos);
				LogPrintDebug("[TextureManager::LoadTexture] Checking if is PVR ");
				char pvr[4];
				in_.read((char*)&pvr, 3);
				pvr[3] = '\0';
				if (strcmp((char*)pvr, "PVR") == 0) {
					LogPrintDebug("[TextureManager::LoadTexture] Is PVR");
					return file_format::PVR;
				}
				else {
					LogPrintDebug("[TextureManager::LoadTexture] Is not PVR");
				}
			}

			return 0;
		}
		
	}
}