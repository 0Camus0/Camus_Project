#ifndef CA_TECHNIQUE_H
#define CA_TECHNIQUE_H

#include <Driver/RenderState.h>
#include <Utils/Maths.h>
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

			virtual	void	SetBool(std::string handler, bool &) = 0;
			virtual	void	SetInt(std::string handler, int &) = 0;
			virtual	void	SetFloat(std::string handler, float &) = 0;
			virtual	void	SetVec2(std::string handler, XVECTOR2 &) = 0;
			virtual	void	SetVec3(std::string handler, XVECTOR3 &) = 0;
			virtual	void	SetMat2(std::string handler, float*) = 0;
			virtual	void	SetMat3(std::string handler, float*) = 0;
			virtual	void	SetMat4(std::string handler, XMATRIX44 &) = 0;

		private:
			std::string		Name;
			std::string		Shader;


		};

	}
}


#endif