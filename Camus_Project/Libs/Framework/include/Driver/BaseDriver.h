#ifndef CA_BASEDRIVER_H
#define CA_BASEDRIVER_H

#include <cstdint>

namespace hyperspace {
	namespace video {

		// Render states.

		enum draw_order_{
			CW,
			CCW,
		};

		enum cull_face_ {
			FRONT,
			BACK,
			FRONT_AND_BACK
		};

		enum blending_function_ {
			ZERO,
			ONE,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,
			SRC_ALPHA_SATURATE
		};

		enum blending_equation_ {
			ADD,
			BLEND_EQ,
			BLEND_EQ_RGB,
			BLEND_EQ_ALPHA,
			SUBSTRACT,
			REVERSE_SUBSTRACT,
			BLEND_MIN,
			BLEND_MAX,
			BLEND_DST_RGB,
			BLEND_SRC_RGB,
			BLEND_DST_ALPHA,
			BLEND_SRC_ALPHA,
			CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA,
			BLEND_COLOR			
		};

		enum capability_ {
			TEXTURE2D = 2048,
			CULL_FACE = 4096,
			BLEND	  = 8192,
			STENCIL	  = 16384,
			DEPTH	  = 32768
		};

		enum draw_bits_ {
			COLOR_BIT = 256,
			DEPTH_BIT = 512,
			STENCIL_BIT = 1024
		};

		enum draw_mask_ {
			RED		= 2,
			GREEN	= 4,
			BLUE	= 8,
			ALPHA	= 16
		};

		enum comp_function_ {
			NEVER,
			LESS,
			EQUAL,
			LEQUAL,
			GREATER,
			NOTEQUAL,
			GEQUAL,
			ALWAYS
		};

		enum texture_filtering_ {
			NEAREST,
			LINEAR,
			ANISOTROPIC,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR
		};

		enum texture_wrap_ {
			REPEAT,
			CLAMP,
			MIRROR
		};
		
		enum texture_unit_ {
			TEX_0,
			TEX_1,
			TEX_2,
			TEX_3,
			TEX_4,
			TEX_5,
			TEX_6,
			TEX_7,
			TEX_8,
			TEX_9,
			TEX_10,
			TEX_11,
			TEX_12,
			TEX_13,
			TEX_14,
			TEX_15,
			TEX_16,
			TEX_17,
			TEX_18,
			TEX_19,
			TEX_20,
			TEX_21,
			TEX_22,
			TEX_23,
			TEX_24,
			TEX_25,
			TEX_26,
			TEX_27,
			TEX_28,
			TEX_29,
			TEX_30
		};

		enum screen_buffers_ {
			BACK_BUFFER,
			FRONT_BUFFER,
			DEPTH_BUFFER,
			RENDER_TARGET_0,
			RENDER_TARGET_1,
			RENDER_TARGET_2,
			RENDER_TARGET_3,
			RENDER_TARGET_4,
			RENDER_TARGET_5,
			RENDER_TARGET_6,
			RENDER_TARGET_7
		};


		class BaseDriver {
		public:
			virtual	void	InitDriver() = 0;
			virtual void	CreateSurfaces() = 0;
			virtual void	DestroySurfaces() = 0;
			virtual void	Update() = 0;
			virtual void	DestroyDriver() = 0;
			virtual void	SetWindow(void *window) = 0;

			
			virtual void	Clear(draw_bits_ mask, int r, int g, int b, int a,float depth = 1.0f) = 0;
			virtual void	Clear(draw_bits_ mask, float r, float g, float b, float a, float depth = 1.0f) = 0;
			virtual void	SwapBuffers() = 0;
		};
	}
}

#endif