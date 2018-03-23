#ifndef T1000_SHADER_BASE_H
#define T1000_SHADER_BASE_H

namespace t1000 {
	namespace shader {
		enum stage_ {
			VERTEX_SHADER = 0,
			PIXEL_SHADER,
			GEOMETRY_SHADER,
			TESSELLATION,
      UNDEFINED
		};

		enum semantic_{
			ATTRIBUTE = 0,
			UNIFORM,
			VARYING,
			UNKNOWN_SEMANTIC
		};

		enum datatype_ {
			INT_ = 0,
			FLOAT_,
			BOOLEAN_,
			VECTOR2_,
			VECTOR3_,
			VECTOR4_,
			MAT2_,
			MAT3_,
			MAT4_,
			SAMPLER1D_,
			SAMPLER2D_,
			SAMPLER3D_,
			SAMPLERSHADOW_,
			SAMPLERCUBE_,
			UNKNOWN_TYPE
		};
	}

}
#endif