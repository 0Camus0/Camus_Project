#ifndef CA_TEXTURE_MANAGER_H
#define CA_TEXTURE_MANAGER_H

#include <Driver/Texture.h>

namespace hyperspace {
	namespace video {
		class TextureManager {
		public:
			TextureManager();

			Texture*		LoadTexture(std::string filename, unsigned int params = GENERATE_MIPMAPS | FILTER_LINEAR_MIPMAP_LINEAR | FILTER_ANISOTROPIC | WRAP_CLAMP);
			Texture*		LoadBufferUncompressed(std::string &Path, unsigned int format, unsigned int params);
			Texture*		LoadBufferCompressed(std::string &Path, unsigned int format, unsigned int params);

			unsigned int	AddTextureToLoadingQueue(std::string filename, unsigned int params = GENERATE_MIPMAPS | FILTER_LINEAR_MIPMAP_LINEAR | FILTER_ANISOTROPIC | WRAP_CLAMP);
			unsigned int	LoadBufferUncompressedQueue(unsigned int &index);
			unsigned int    LoadBufferCompressedQueue(unsigned int &index);
			void			LoadTextureQueue();

			unsigned int	CheckFormat(std::ifstream &in);
			bool			CheckIfExtensionIsSupported(char *name, unsigned int &props);

#if USE_LOG_DEBUG_TEX_LOADING
			void	PrintTextureInfo(std::string &name, Texture *tex);
#endif
			~TextureManager() {

			}

			static unsigned int		num_textures_loaded;
			static unsigned int		current_index;

			static unsigned int		queue[MAX_TEXURE_LIMIT];
			static unsigned int		queueoffsets[MAX_TEXURE_LIMIT];		
			static unsigned char	tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
			static			char	tex_paths_pool[MAX_TEXURE_LIMIT][MAX_PATH_SIZE];
			static Texture		   *textures[MAX_TEXURE_LIMIT];
		};
	}
}

#endif