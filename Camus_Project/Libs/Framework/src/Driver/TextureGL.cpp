#include <Driver/TextureGL.h>
#include <Driver/DriverProperties.h>
#include <Utils/Log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifdef OS_ANDROID
#ifndef GL_IMG_texture_compression_pvrtc2
#define GL_IMG_texture_compression_pvrtc2 1
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG 0x9137
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG 0x9138
#endif
#endif


#include <algorithm>

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

		void TextureManagerGL::GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp) {

			if (props & compress_format::PVRTC2) {
				if (props & channelS_::CH_RGB) {
					glFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;	
				}
				else if (props & channelS_::CH_RGBA) {
					glFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
				}
				bpp = 2;
			}

			if (props & compress_format::PVRTC4) {
				if (props & channelS_::CH_RGB) {
					glFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				}
				else if (props & channelS_::CH_RGBA) {
					glFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
				}
				bpp = 4;
			}

			if (props & compress_format::PVRTCII2) {
				glFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG;
				bpp = 2;
			}
			
			if (props & compress_format::PVRTCII4) {
				glFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG;
				bpp = 4;
			}
			
			if (props & compress_format::ETC1) {
				glFormat = GL_ETC1_RGB8_OES;
				bpp = 4;
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

			tex->id = static_cast<unsigned short>(id);
			tex->params = params;

		}

		void TextureManagerGL::LoadAPITextureCompressed(Texture *tex, unsigned char* buffer, unsigned int &params) {
			unsigned int id;
			unsigned int glFormat = 0;
			unsigned int mipmaps_count = tex->mipmaps;
			unsigned int props = tex->props;
			unsigned int bpp = 0;
			unsigned int current_x = tex->x, current_y = tex->y;
			unsigned int offset = 0;
			unsigned int min_size = 1;

			GetFormatBpp(props, glFormat, bpp);

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			for (unsigned int i = 0; i < mipmaps_count; i++) {
				unsigned int size_ = std::max((current_x*current_y*bpp) / 8, min_size);
				glCompressedTexImage2D(GL_TEXTURE_2D, i, glFormat, current_x, current_y, 0, size_, (void*)(buffer + offset));
				offset += size_;
				current_x = current_x >> 1;
				current_y = current_y >> 1;
			}

			SetTextureParams(params);

			tex->id = static_cast<unsigned short>(id);
			tex->params = params;
		}
	}
}