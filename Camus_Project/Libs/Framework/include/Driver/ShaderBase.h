#ifndef GLSL_BASE_H
#define GLSL_BASE_H

#include <iostream>
#include <string>

namespace hyperspace {
	namespace video {
		namespace shader {
			enum stage_ {
				VERTEX_SHADER		= (1 << 0),
				PIXEL_SHADER		= (1 << 1),
				GEOMETRY_SHADER		= (1 <<	2),
				TESSELLATION		= (1 <<	3)
			};

			enum semantic_ {
				ATTRIBUTE			= (1 << 4),
				UNIFORM				= (1 << 5),
				VARYING				= (1 << 6),
			};

			enum datatype_ {
				INT_			= (1 << 7),
				FLOAT_			= (1 << 8),
				BOOLEAN_		= (1 << 9),
				VECTOR2_		= (1 << 10),
				VECTOR3_		= (1 << 11),
				VECTOR4_		= (1 << 12),
				MAT2_			= (1 << 13),
				MAT3_			= (1 << 14),
				MAT4_			= (1 << 15),
				SAMPLER1D_		= (1 << 16),
				SAMPLER2D_		= (1 << 17),
				SAMPLER3D_		= (1 << 18),
				SAMPLERSHADOW_  = (1 << 19),
				SAMPLERCUBE_	= (1 << 20),
			};

			enum bind_{
				POSITION		= (1 << 21),
				NORMAL			= (1 << 22),
				TEXCOORD_1		= (1 << 23),
				TEXCOORD_2		= (1 << 24),
				TEXCOORD_3		= (1 << 25),
				TEXCOORD_4		= (1 << 26),
				TANGENT			= (1 << 27),
				BINORMAL		= (1 << 28),
				BONE_WEIGHTS	= (1 << 29),
				BONE_INDICES	= (1 << 30),
			};

			class Shader_Var_ {
			public:
				Shader_Var_() : props(0) , loc(0) {
				}
				~Shader_Var_(){
				}
				unsigned int		props;
				unsigned int		loc;
				std::string			name;

				bool operator ==(const Shader_Var_ &o) const;
				bool operator !=(const Shader_Var_ &o) const;
				bool operator <(const Shader_Var_ &o) const;
				bool operator >(const Shader_Var_ &o) const;
				
			};
		}
	}
}
#endif