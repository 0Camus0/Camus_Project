#include <Driver/ShaderBase.h>
#include <Utils/Log.h>

namespace hyperspace {
	namespace video {
		namespace shader {
			bool Shader_Var_::operator == (const Shader_Var_ &o) const {
				return (name == o.name);
			}

			bool Shader_Var_::operator != (const Shader_Var_ &o)  const {
				return !(name == o.name);
			}

			bool Shader_Var_::operator < (const Shader_Var_ &o)  const {
				return (name < o.name);
			}

			bool Shader_Var_::operator > (const Shader_Var_ &o) const {
				return (name > o.name);
			}
		}
	}
}