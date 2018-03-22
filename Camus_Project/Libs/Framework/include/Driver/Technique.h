#ifndef CA_TECHNIQUE_H
#define CA_TECHNIQUE_H

#include <Driver\RenderState.h>
#include <Driver\ShaderBase.h>
#include <Utils\Maths.h>
#include <string>
#include <vector>
#include <map>
#include <cstddef>

namespace hyperspace {
	namespace video {

		typedef std::map<std::string, shader::Shader_Var_>	   Handlers;

		class ArgumentsManager {
		public:
			ArgumentsManager() {
				arguments = "";
			
			}
			ArgumentsManager(std::string str) {
			
				if (!str.empty()) {
					if (str[str.size() - 1] != ';') {
						str += ";";
					}
					std::size_t pos = str.find(";");
					std::size_t length = 0;
					std::size_t tempPos = 0;
					while (pos != std::string::npos) {
						length = pos - tempPos;
						defines.push_back(str.substr(tempPos, length));
						tempPos = pos + 1;
						pos = str.find(";", pos + 1);
					}
				}
			}
			~ArgumentsManager() {
				
			}
			void AddDefine(std::string arg) {
				defines.push_back(arg);
			}
			void ClearArguments(){
				defines.clear();
				arguments.clear();
			}
			std::string	 BuildArgumentListGLStyle() {
				if (defines.size() == 0) {
					return "";
				}
				arguments = "\n\n";
				for (std::size_t i = 0; i < defines.size(); i++) {
					arguments += "#define " + defines[i] + "\n";
				}
				arguments += "\n";
				return arguments;
			}
			std::string	 BuildArgumentListDXStyle() {
				arguments = "";
				for (std::size_t i = 0; i < defines.size(); i++) {
					arguments += "-D " + defines[i] + " ";
				}
				return arguments;
			}

			std::vector <std::string>	defines;
			std::string					arguments;
		};

		struct Pass_ {
			Pass_() : vertexID(0), pixelID(0), program(0){  }
			~Pass_(){
			
			}

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
			CTechnique_() {
		
			}
			virtual void			Initialize(std::string name, std::string shader, ArgumentsManager args = ArgumentsManager() ) = 0;
			virtual void			AddPass(std::string name, ArgumentsManager args,std::string path,CRenderStateDesc desc) = 0;
			virtual void			RemovePass(std::size_t id) = 0;
			virtual std::int32_t	GetNumPasses() = 0;
			virtual void			SetPass(std::size_t id) = 0;
			virtual bool			BindAttribute(std::string name,unsigned int pass,unsigned int binding) = 0;
			virtual unsigned int	CompileShader(shader::stage_ type, std::string path, std::string args) = 0;

			virtual	void	SetBool(std::string handler, bool &) = 0;
			virtual	void	SetInt(std::string handler, int &) = 0;
			virtual	void	SetFloat(std::string handler, float &) = 0;
			virtual	void	SetVec2(std::string handler, XVECTOR2 &) = 0;
			virtual	void	SetVec3(std::string handler, XVECTOR3 &) = 0;
			virtual	void	SetVec4(std::string handler, XVECTOR3 &) = 0;
			virtual	void	SetMat2(std::string handler, float*) = 0;
			virtual	void	SetMat3(std::string handler, float*) = 0;
			virtual	void	SetMat4(std::string handler, XMATRIX44 &) = 0;

			virtual ~CTechnique_() {
			
			}

			std::string			Name;
			std::string			Shader;
			std::vector<Pass_>	Passes;

		};

	

	}
}


#endif