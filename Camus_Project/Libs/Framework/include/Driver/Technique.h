#ifndef CA_TECHNIQUE_H
#define CA_TECHNIQUE_H

#include <Driver/RenderState.h>
#include <string>
#include <cstddef>

namespace hyperspace {
	namespace video {

		struct Pass_ {
			std::string			name;
			std::string			args;
			CRenderStateDesc	renderstate;
		};

		class CTechnique {
		public:
			CTechnique() {}

			virtual void			Initialize(std::string name, std::string shader) = 0;
			virtual void			AddPass(Pass_ pass) = 0;
			virtual void			RemovePass(int id) = 0;
			virtual std::int32_t	GetNumPasses() = 0;

		private:
			std::string		Name;
			std::string		Shader;


		};

	}
}


#endif