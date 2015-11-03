#include <Driver/Texture.h>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

#include <Driver/PNG/lodepng.h>
#include <vector>

namespace hyperspace {
	namespace video {
		Texture			TextureManager::textures[MAX_TEXURE_LIMIT];
		unsigned char	TextureManager::tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
		unsigned char	TextureManager::tex_names_pool[MAX_TEXURE_LIMIT][32];

		unsigned short	TextureManager::LoadTexture(std::string filename) {
			LogPrintDebug("[TextureManager::LoadTexture] %s ", filename.c_str());

			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Textures/";
			Path += filename;

			std::ifstream in_(Path.c_str(), std::ios::binary | std::ios::in);
			if (!in_.good()) {
				in_.close();
				return TEXTURE_NOT_FOUND;
			}

			unsigned int format = CheckFormat(in_);

			in_.close();

			if (format == 0) {

				return TEXTURE_NOT_FOUND;
			}

			
			
			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (!textures[i].bounded) {
					Texture *tex = 0;
					int id = i;
					tex = &textures[id];
					
					
					if (format&file_format::PNG) {
						LodePNGColorType type;			
						unsigned int x = 0, y = 0;
						unsigned int bpp = 0;
						std::vector<unsigned char> data;
						unsigned int error = lodepng::decode(data,x,y,Path,type,bpp);
						LogPrintDebug("[TextureManager::LoadTexture] Size: %d  %d  %d  bytes size: %d error[%s]",x,y,bpp,data.size(), lodepng_error_text(error));
						break;
					}


				}
			}

			
			

			return TEXTURE_FOUND;
		}


		unsigned int TextureManager::CheckFormat(std::ifstream &in_) {

			std::streampos begPos = in_.tellg();

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

			return 0;
		}
		
	}
}