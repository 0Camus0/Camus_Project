#ifndef CA_TEXTURE_GL_H
#define CA_TEXTURE_GL_H

#include <Driver/Texture.h>

namespace hyperspace {
	namespace video {
		class TextureManagerGL : public TextureManager {
		public:
			TextureManagerGL(){}

			void	SetTextureParams(unsigned int &params,unsigned int &target);
			void	GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp);

			void	LoadAPITexture(Texture *tex, unsigned char* buffer, unsigned int &params);
			void	LoadAPITextureCompressed(Texture *tex, unsigned char* buffer, unsigned int &params);
		};
	}
}

#endif