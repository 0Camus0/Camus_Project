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

		typedef std::map<shader::Shader_Var_, int> Handlers;

		class ArgumentsManager {
		public:
			ArgumentsManager() {
				arguments = "";
			}
			void AddArgument(std::string arg) {
				individuals.push_back(arg);
			}
			void ClearArguments(){
				individuals.clear();
				arguments.clear();
			}
			std::string	 BuildArgumentListGLStyle() {
				if (individuals.size() == 0) {
					return "";
				}
				arguments = "\n\n";
				for (std::size_t i = 0; i < individuals.size(); i++) {
					arguments += "#define " + individuals[i] + "\n";
				}
				arguments += "\n";
				return arguments;
			}
			std::string	 BuildArgumentListDXStyle() {
				arguments = "";
				for (std::size_t i = 0; i < individuals.size(); i++) {
					arguments += "-D " + individuals[i] + " ";
				}
				return arguments;
			}

			std::vector <std::string>	individuals;
			std::string					arguments;
		};

		struct Pass_ {
			Pass_() : vertexID(0), pixelID(0), program(0){}
			int						vertexID;
			int						pixelID;
			int						program;
			CRenderStateDesc		renderstate;
			std::string				name;	
			std::string				path;
			ArgumentsManager		args;
			Handlers				handlers;
		};

		class CTechnique_ {
		public:
			CTechnique_() {}
			virtual void			Initialize(std::string name, std::string shader) = 0;
			virtual void			AddPass(std::string name, ArgumentsManager args,std::string path,CRenderStateDesc desc) = 0;
			virtual void			RemovePass(std::size_t id) = 0;
			virtual std::int32_t	GetNumPasses() = 0;
			virtual void			SetPass(std::size_t id) = 0;
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