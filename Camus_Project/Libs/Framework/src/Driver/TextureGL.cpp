#include <Driver/TextureGL.h>
#include <Driver/DriverProperties.h>
#include <Utils/Log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace hyperspace {
	namespace video {

		void	TextureManagerGL::SetTextureParams(unsigned int &params) {
			unsigned int glFiltering = 0;
			unsigned int glWrap = 0;

			if (params&params_in::GENERATE_MIPMAPS) {
				if (params & params_in::FILTER_NEAREST_MIPMAP_NEAREST)
					glFiltering = GL_NEAREST_MIPMAP_NEAREST;
				if (params & params_in::FILTER_NEAREST_MIPMAP_LINEAR)
					glFiltering = GL_NEAREST_MIPMAP_LINEAR;
				if (params & params_in::FILTER_LINEAR_MIPMAP_NEAREST)
					glFiltering = GL_LINEAR_MIPMAP_NEAREST;
				if (params & params_in::FILTER_LINEAR_MIPMAP_LINEAR)
					glFiltering = GL_LINEAR_MIPMAP_LINEAR;
			}
			else {
				if (params & (params_in::FILTER_NEAREST | params_in::FILTER_NEAREST_MIPMAP_LINEAR | params_in::FILTER_NEAREST_MIPMAP_NEAREST))
					glFiltering = GL_NEAREST;
				if (params & (params_in::FILTER_LINEAR | params_in::FILTER_LINEAR_MIPMAP_NEAREST | params_in::FILTER_LINEAR_MIPMAP_LINEAR))
					glFiltering = GL_LINEAR;
			}

			if (params&params_in::WRAP_CLAMP)
				glWrap = GL_CLAMP_TO_EDGE;
			if (params&params_in::WRAP_REPEAT)
				glWrap = GL_REPEAT;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFiltering);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFiltering);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);

			if (params&params_in::FILTER_ANISOTROPIC) {
				if (GetDriverProperties().isExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
					int Max = 1;
					glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Max);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Max);
				}
			}
		}

		void TextureManagerGL::LoadAPITexture(Texture *tex, unsigned char* buffer, unsigned int &params) {
			unsigned int id;
			unsigned int glFormat = 0;
			unsigned int glChannel = GL_UNSIGNED_BYTE;
			
			if (tex->props&channelS_::CH_ALPHA)
				glFormat = GL_ALPHA;
			else if (tex->props&channelS_::CH_RGB)
				glFormat = GL_RGB;
			else if (tex->props&channelS_::CH_RGBA)
				glFormat = GL_RGBA;

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			if (tex->x % 4 != 0)
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			else
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			glTexImage2D(GL_TEXTURE_2D, 0, glFormat, tex->x, tex->x, 0, glFormat, glChannel, (void*)(buffer));

			if (params&params_in::GENERATE_MIPMAPS)
				glGenerateMipmap(GL_TEXTURE_2D);

			SetTextureParams(params);

			tex->id = id;
			tex->params = params;

		}

		void TextureManagerGL::LoadAPITextureCompressed(Texture *tex, unsigned char* buffer, unsigned int &params) {
	
		}
	}
}