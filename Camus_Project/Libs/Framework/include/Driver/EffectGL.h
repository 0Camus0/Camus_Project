#ifndef EFFECT_GL_H
#define EFFECT_GL_H

#include <Driver/Technique.h>
#include <Driver/GLSLParser.h>


namespace hyperspace {
	namespace video {

		class TechniqueGL : public CTechnique_ {
		public:
			TechniqueGL() : CurrentPass(0) {
			}
			 void			Initialize(std::string name, std::string shader, ArgumentsManager args = ArgumentsManager());
			 void			AddPass(std::string name, ArgumentsManager args, std::string path, CRenderStateDesc desc);
			 void			RemovePass(std::size_t id);
			 std::int32_t	GetNumPasses();
			 void			SetPass(std::size_t id);
			 bool			BindAttribute(std::string name, unsigned int pass, unsigned int binding);

			 void			SetBool(std::string handler, bool &);
			 void			SetInt(std::string handler, int &);
			 void			SetFloat(std::string handler, float &);
			 void			SetVec2(std::string handler, XVECTOR2 &);
			 void			SetVec3(std::string handler, XVECTOR3 &);
			 void			SetVec4(std::string handler, XVECTOR3 &);
			 void			SetMat2(std::string handler, float*);
			 void			SetMat3(std::string handler, float*);
			 void			SetMat4(std::string handler, XMATRIX44 &);

			 void			DebugPassesContent();

			 ~TechniqueGL() {
			 }

			 GLSL_Parser	Parser;
			 Pass_			*CurrentPass;
			 

		private:
			unsigned int	CompileShader(shader::stage_ type, std::string path, std::string args);
			void			GetHandlers(Pass_ &pass);
			void			CreateProgram(Pass_ &pass);
		};

	}
}


#endif