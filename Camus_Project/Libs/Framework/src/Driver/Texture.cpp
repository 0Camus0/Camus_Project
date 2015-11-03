#include <Driver/Texture.h>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

#include <Driver/PNG/lodepng.h>
#include <vector>

namespace hyperspace {
	namespace video {
		Texture			TextureManager::textures[MAX_TEXURE_LIMIT];
		unsigned char	TextureManager::tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
				 char	TextureManager::tex_paths_pool[MAX_TEXURE_LIMIT][512];

		unsigned int	TextureManager::num_textures_loaded = 0;
		unsigned int	TextureManager::current_index = 0;

		TextureManager::TextureManager(){
			MemAppendHeap(TextureManager);
		}

		unsigned short	TextureManager::LoadTexture(std::string filename) {
			LogPrintDebug("[TextureManager::LoadTexture] %s ", filename.c_str());

			unsigned short ret = TEXTURE_NOT_FOUND;

			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Textures/";
			Path += filename;

			std::ifstream in_(Path.c_str(), std::ios::binary | std::ios::in);
			if (!in_.good()) {
				in_.close();
				LogPrintError("[TextureManager::LoadTexture] %s not found ", Path.c_str());
				return ret;
			}

			unsigned int format = CheckFormat(in_);

			in_.close();

			if (format == 0) {
				LogPrintError("[TextureManager::LoadTexture] %s cannot determine format.", Path.c_str());
				return ret;
			}
		
			if (format == file_format::PNG) {
				ret = LoadPNG(Path);
			}
			
			

			return ret;
		}

		unsigned int TextureManager::LoadPNG(std::string &Path) {

			int id = 0;
			int offset = 0;
			for (int i = 0; i < MAX_TEXURE_LIMIT; i++) {
				if (!textures[i].bounded) {
					id = i;
					break;
				}else{
					offset += textures[i].size;
				}
			}

			Texture *tex = &textures[id];
			unsigned int x = 0, y = 0, channels = 0;
			lodepng::State st;
			std::vector<unsigned char> data;
			std::vector<unsigned char> png;
			lodepng::load_file(png, Path.c_str());
			unsigned int error = lodepng::decode(data, x, y, st, png);

			if (error != 0) {
				LogPrintError("[TextureManager::LoadTexture] Failed at loading textures [%s] ", lodepng_error_text(error));
				return TEXTURE_NOT_FOUND;
			}
			
			tex->bounded = 1;
			tex->id = id;
			tex->x = x;
			tex->y = y;
			tex->size = data.size();
			tex->offset = offset;
			tex->props |= file_format::PNG;
			tex->props |= compress_format::RAW;

			if (st.info_raw.bitdepth == 8)
				tex->props |= pixel_format_::UINTEGER_8;

			switch (st.info_raw.colortype) {
			case LCT_GREY: {
				tex->props |= channelS_::CH_LUMINANCE;
				channels = 1;
			}break;
			case LCT_GREY_ALPHA: {
				tex->props |= channelS_::CH_ALPHA;
				channels = 1;
			}break;
			case LCT_RGB: {
				tex->props |= channelS_::CH_RGB;
				channels = 3;
			}break;
			case LCT_RGBA: {
				tex->props |= channelS_::CH_RGBA;
				channels = 4;
			}break;
			}

				
			if (Path.size() > 32) {
				std::string sub = Path.substr(0, 511);
				strcpy_s(tex_paths_pool[id], sub.c_str());
				tex_paths_pool[id][511] = '\0';
			}
			else {
				strcpy_s(tex_paths_pool[id], Path.c_str());
				tex_paths_pool[id][Path.size()-1] = '\0';
			}

			memcpy((void*)(tex_mem_pool + offset), (void*)&data[0], tex->size);
			
			current_index = id;
			num_textures_loaded++;

			LogPrintDebug("[TextureManager::LoadTexture] Loaded [%s] info: [%d x %d x %d]  %d kb.", tex_paths_pool[id], x, y, channels*st.info_raw.bitdepth, tex->size / 1024);
			
			LogPrintDebug("Stack allocated mem: %d ", TEX_STACK_ALLOCATED_SIZE);

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