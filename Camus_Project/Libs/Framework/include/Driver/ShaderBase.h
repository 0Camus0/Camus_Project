#ifndef GLSL_BASE_H
#define GLSL_BASE_H

#include <iostream>
#include <string>
#include <Utils/MemoryTracker.h>

namespace hyperspace {
	namespace video {
		namespace shader {
			enum stage_ {
				VERTEX_SHADER = 0,
				PIXEL_SHADER,
				GEOMETRY_SHADER,
				TESSELLATION,
			};

			enum semantic_ {
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

			class Shader_Var_ {
			public:
				Shader_Var_() {
					MemAppendHeap(Shader_Var_);
				}
				shader::semantic_	sem;
				shader::datatype_	type;
				shader::stage_		stage;
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