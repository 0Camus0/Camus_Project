#ifndef CA_TEXTURE_GL_H
#define CA_TEXTURE_GL_H

#include <Driver\Texture.h>

namespace hyperspace {
	namespace video {
		class TextureGL : public Texture {
		public:
			TextureGL(){}

			void	SetTextureParams(unsigned int &params,unsigned int &target);
			void	GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp);

			void	LoadAPITexture(unsigned char* buffer);
			void	LoadAPITextureCompressed(unsigned char* buffer);
		};
	}
}

#endif