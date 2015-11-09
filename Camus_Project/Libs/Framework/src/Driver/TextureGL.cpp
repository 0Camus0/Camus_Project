#include <Driver/TextureGL.h>
#include <Driver/DriverProperties.h>
#include <Utils/Log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>


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

		void	TextureManagerGL::SetTextureParams(unsigned int &params, unsigned int &target) {
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

			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, glFiltering);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, glFiltering);
			glTexParameteri(target, GL_TEXTURE_WRAP_S, glWrap);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, glWrap);

			if (params&params_in::FILTER_ANISOTROPIC) {
				if (GetDriverProperties().isExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
					int Max = 1;
					glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Max);
					glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, Max);
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
				return;
			}

			if (props & compress_format::PVRTC4) {
				if (props & channelS_::CH_RGB) {
					glFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				}
				else if (props & channelS_::CH_RGBA) {
					glFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
				}
				bpp = 4;
				return;
			}

			if (props & compress_format::PVRTCII2) {
				glFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG;
				bpp = 2;
				return;
			}
			
			if (props & compress_format::PVRTCII4) {
				glFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG;
				bpp = 4;
				return;
			}
			
			if (props & compress_format::ETC1) {
				glFormat = GL_ETC1_RGB8_OES;
				bpp = 4;
			}

			if (props & compress_format::ETC2) {
				glFormat = GL_COMPRESSED_RGBA8_ETC2_EAC;
				bpp = 8;
			}

			if (props & compress_format::DXT1) {
				glFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				bpp = 4;
			}

			if (props & compress_format::DXT5) {
				glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				bpp = 8;
			}
		}

		void TextureManagerGL::LoadAPITexture(Texture *tex, unsigned char* buffer, unsigned int &params) {
			unsigned int id;
			unsigned int glFormat = 0;
			unsigned int glChannel = GL_UNSIGNED_BYTE;
			unsigned int glTarget = GL_TEXTURE_2D;
			
			if (tex->props&channelS_::CH_ALPHA)
				glFormat = GL_ALPHA;
			else if (tex->props&channelS_::CH_RGB)
				glFormat = GL_RGB;
			else if (tex->props&channelS_::CH_RGBA)
				glFormat = GL_RGBA;

			glGenTextures(1, &id);
			glBindTexture(glTarget, id);

			if (tex->x % 4 != 0)
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			else
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			glTexImage2D(glTarget, 0, glFormat, tex->x, tex->x, 0, glFormat, glChannel, (void*)(buffer));

			if (params&params_in::GENERATE_MIPMAPS)
				glGenerateMipmap(glTarget);

			SetTextureParams(params, glTarget);

			tex->id = static_cast<unsigned short>(id);
			tex->params = params;

		}

		void TextureManagerGL::LoadAPITextureCompressed(Texture *tex, unsigned char* buffer, unsigned int &params) {
			unsigned int id;
			unsigned int glFormat = 0;
			unsigned int mipmaps_count = tex->mipmaps;
			unsigned int props = tex->props;
			unsigned int bpp = 0;
			int current_x = tex->x, current_y = tex->y;
			unsigned int glTarget = 0;
			unsigned int glfaceTarget = 0;
			unsigned int faces = (props&props_::CUBE_MAP) ? 6 : 1;

			if (faces == 6) {
				glTarget = GL_TEXTURE_CUBE_MAP;
				glfaceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			}
			else {
				glTarget = GL_TEXTURE_2D;
				glfaceTarget = glTarget;
			}	

			GetFormatBpp(props, glFormat, bpp);

			glGenTextures(1, &id);
			glBindTexture(glTarget, id);
			unsigned char *pbuff = buffer;

			int blockSize = 0, widthBlocks = 0, heightBlocks = 0;
			if (props & bpp_::BPP_4) {
				blockSize = 4 * 4;
				widthBlocks = current_x / 4;
				heightBlocks = current_y / 4;
			}else if (props & bpp_::BPP_2) {
				blockSize = 8 * 4;
				widthBlocks = current_x / 8;
				heightBlocks = current_y / 4;
			}else if (props & bpp_::BPP_8) {
				blockSize = 8 * 8;
				widthBlocks = current_x / 8;
				heightBlocks = current_y / 8;
			}
			int size_ = 0;
			for (unsigned int i = 0; i < mipmaps_count; i++) {		
				widthBlocks = widthBlocks < 2 ? 2 : widthBlocks;
				heightBlocks = heightBlocks < 2 ? 2 : heightBlocks;

				if (props&compress_format::ETC1) {
					size_ = (current_x*current_y*bpp) / 8;
					size_ = std::max(size_, 8);
				}else {
					size_ = widthBlocks * heightBlocks * ((blockSize * bpp) / 8);
				}
				for (unsigned int f = 0; f < faces; f++) {
					glCompressedTexImage2D(glfaceTarget + f, i, glFormat, current_x, current_y, 0, size_, pbuff);
					pbuff += size_;
				}

				current_x = std::max(current_x >> 1, 1);
				current_y = std::max(current_y >> 1, 1);

				if (props & bpp_::BPP_4) {
					widthBlocks = current_x / 4;
					heightBlocks = current_y / 4;
				}
				else if (props & bpp_::BPP_2) {
					widthBlocks = current_x / 8;
					heightBlocks = current_y / 4;
				}
				else if (props & bpp_::BPP_8) {
					widthBlocks = current_x / 8;
					heightBlocks = current_y / 8;
				}
			}

			SetTextureParams(params, glTarget);

			tex->id = static_cast<unsigned short>(id);
			tex->params = params;
		}
	}
}