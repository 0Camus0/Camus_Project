#ifndef CA_TECHNIQUE_H
#define CA_TECHNIQUE_H

#include <Driver/RenderState.h>
#include <Driver/ShaderBase.h>
#include <Utils/Maths.h>
#include <string>
#include <vector>
#include <map>
#include <cstddef>

namespace hyperspace {
	namespace video {

		struct Pass_ {
			Pass_() : pVertexShader(0),pPixelShader(0) {}
			std::string				name;
			std::string				args;
			CRenderStateDesc		renderstate;
			shader::CVertexShader_	*pVertexShader;
			shader::CPixelShader_	*pPixelShader;
		};

		class CTechnique_ {
		public:
			CTechnique_() {}
			virtual void			Initialize(std::string name, std::string shader) = 0;
			virtual void			AddPass(Pass_ pass) = 0;
			virtual void			RemovePass(int id) = 0;
			virtual std::int32_t	GetNumPasses() = 0;
			virtual void			SetPass(int index) = 0;
			virtual unsigned int	CompileShader(shader::stage_ type, std::string path, std::string args) = 0;


			std::string			Name;
			std::string			Shader;
			std::vector<Pass_>	Passes;

		};

		class CEffect_ {
		public:
			virtual void	AddTechnique(CTechnique_* tech) = 0;
			virtual void	RemoveTechnique(std::string name) = 0;

			virtual	void	SetBool(std::string handler, bool &) = 0;
			virtual	void	SetInt(std::string handler, int &) = 0;
			virtual	void	SetFloat(std::string handler, float &) = 0;
			virtual	void	SetVec2(std::string handler, XVECTOR2 &) = 0;
			virtual	void	SetVec3(std::string handler, XVECTOR3 &) = 0;
			virtual	void	SetMat2(std::string handler, float*) = 0;
			virtual	void	SetMat3(std::string handler, float*) = 0;
			virtual	void	SetMat4(std::string handler, XMATRIX44 &) = 0;

			virtual void	SetTechnique(std::string name) = 0;

			std::vector<CTechnique_*>	Techniques;
		};

	}
}


#endif