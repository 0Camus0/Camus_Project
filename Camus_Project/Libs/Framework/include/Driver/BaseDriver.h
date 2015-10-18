#ifndef CA_BASEDRIVER_H
#define CA_BASEDRIVER_H

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
			TEXTURE2D,
			CULL_FACE,
			BLEND,
			STENCIL,
			DEPTH
		};

		enum draw_bits {
			COLOR,
			DEPTH,
			STENCIL
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

		enum texture_filtering {
			NEAREST,
			LINEAR,
			ANISOTROPIC,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR
		};

		enum texture_parameter {
			MAG_FILTER,
			MIN_FILTER,
			WRAP_S,
			WRAP_T
		};

		enum texture_wrap {
			REPEAT,
			CLAMP,
			MIRROR
		};
		
		enum texture_unit {
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

		class BaseDriver {
		public:
			virtual	void	InitDriver() = 0;
			virtual void	CreateSurfaces() = 0;
			virtual void	DestroySurfaces() = 0;
			virtual void	Update() = 0;
			virtual void	SwapBuffers() = 0;
			virtual void	DestroyDriver() = 0;
			virtual void	SetWindow(void *window) = 0;
		};
	}
}

#endif