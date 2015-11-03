#include <Driver/Texture.h>
#include <Utils/FileSystem.h>



namespace hyperspace {
	namespace texture {
		Texture			TextureManager::textures[MAX_TEXURE_LIMIT];
		unsigned char	TextureManager::tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
		unsigned char	TextureManager::tex_names_pool[MAX_TEXURE_LIMIT][32];

		unsigned short	TextureManager::LoadTexture(std::string filename) {
			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Textures/";
			Path += filename;

			std::ifstream in_(Path.c_str(), std::ios::binary | std::ios::in);
			if (!in_.good()) {
				in_.close();
				return NOT_FOUND_TEXTURE;
			}

			unsigned short ret = ProcessHeader(in_);

		}

		unsigned short	ProcessHeader(const std::ifstream &stream);
	}
}