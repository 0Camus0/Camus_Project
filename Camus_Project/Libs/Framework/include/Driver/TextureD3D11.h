#ifndef CA_TEXTURE_D3D11_H
#define CA_TEXTURE_D3D11_H

#include <Driver/Texture.h>

namespace hyperspace {
	namespace video {
		class TextureD3D11 : public Texture {
		public:
			TextureD3D11() {}

			void	SetTextureParams(unsigned int &params, unsigned int &target);
			void	GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp);

			void	LoadAPITexture(unsigned char* buffer);
			void	LoadAPITextureCompressed(unsigned char* buffer);
		};
	}
}

#endif