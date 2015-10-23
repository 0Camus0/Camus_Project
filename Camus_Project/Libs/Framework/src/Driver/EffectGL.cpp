#include <Driver/EffectGL.h>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

#include <GLES2/gl2.h>

#include <fstream>

#define SHOW_SHADERS_CONTENT 0

namespace hyperspace {
	namespace video {

		void TechniqueGL::Initialize(std::string name, std::string shader) {
			LogPrintDebug("TechniqueGL::Initialize: %s  shader: %s ", name.c_str(), shader.c_str());

			Name = name;
			Shader = shader;
			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Shaders/";
			Path += shader;

			LogPrintDebug("TechniqueGL::Initialize Path to use: %s " ,Path.c_str());
			Parser.Parse(Path);
			AddPass("0", ArgumentsManager(""), Path, CRenderStateDesc());

		}

		void TechniqueGL::AddPass(std::string name, ArgumentsManager args, std::string path, CRenderStateDesc desc) {
			Pass_ pass;
			pass.args = args;
			pass.name = name;
			pass.path = path;
			pass.vertexID = CompileShader(shader::stage_::VERTEX_SHADER, (pass.path + std::string("_VS.glsl")), pass.args.BuildArgumentListGLStyle());
			pass.pixelID = CompileShader(shader::stage_::PIXEL_SHADER, (pass.path + std::string("_FS.glsl")), pass.args.BuildArgumentListGLStyle());
			CreateProgram(pass);
			GetHandlers(pass);
			Passes.push_back(pass);
			SetPass(0);
		}

		void TechniqueGL::RemovePass(std::size_t id) {
			if (id >= Passes.size())
				return;
		
			Pass_ &pass = Passes[id];
			glDetachShader(pass.program, pass.vertexID);
			glDetachShader(pass.program, pass.pixelID);
			glDeleteProgram(pass.program);
			Passes.erase(Passes.begin() + id);
		}

		std::int32_t TechniqueGL::GetNumPasses() {
			return Passes.size();
		}

		void TechniqueGL::SetPass(std::size_t id) {
			CurrentPass = &Passes[0];
			glUseProgram(CurrentPass->program);
		}

		unsigned int TechniqueGL::CompileShader(shader::stage_ type_, std::string path, std::string args) {
			GLuint type;
			
			if (type_ == shader::stage_::VERTEX_SHADER)
				type = GL_VERTEX_SHADER;
			
			if (type_ == shader::stage_::PIXEL_SHADER)
				type = GL_FRAGMENT_SHADER;

			std::ifstream in_(path.c_str(), std::ios::in| std::ios::binary);

			if (!in_.good()) {
				LogPrintError("TechniqueGL: Shader on Path: %s not found.", path.c_str());
				in_.close();
				return 65535;
			}

			char	*pSource;
			in_.seekg(0, std::ios::end);
			unsigned int Size = static_cast<unsigned int>(in_.tellg());
			in_.seekg(0, std::ios::beg);
			pSource = new char[Size+1];
			in_.read(&pSource[0], Size);
			pSource[Size] = '\0';
			in_.close();

			std::string strSource = args;
			strSource += std::string(pSource);

			LogPrintDebug("Compiling shader [%s] with args [%s] ...", path.c_str(),args.c_str());

#if SHOW_SHADERS_CONTENT
			LogPrintDebug("Content\n[%s]\n", strSource.c_str());
#endif
			unsigned int Id = glCreateShader(type);
			const char *c_str = strSource.c_str();
			glShaderSource(Id, 1,&c_str, NULL);
			glCompileShader(Id);

			int bCompiled;
			glGetShaderiv(Id, GL_COMPILE_STATUS, &bCompiled);
			if (!bCompiled) {
				int LogSize, LogWritten;
				glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &LogSize);
				char* pError = new char[LogSize + 1];
				pError[LogSize] = '\0';
				glGetShaderInfoLog(Id, LogSize, &LogWritten, pError);
				std::string strError = pError;
				if (type == GL_VERTEX_SHADER)
					LogPrintError("Failed to compile vertex shader: %s with args: %s\nLog[%s]", path.c_str(), args.c_str(), strError.c_str());
				if (type == GL_FRAGMENT_SHADER)
					LogPrintError("Failed to compile pixel shader: %s with args: %s\nLog[%s]", path.c_str(), args.c_str(), strError.c_str());
				delete [] pError;				
			}
			else {
				LogPrintDebug("...Success.");
			}

			delete [] pSource;

			return Id;
		}

		void TechniqueGL::CreateProgram(Pass_ &pass) {
			pass.program = glCreateProgram();
			glAttachShader(pass.program, pass.vertexID);
			glAttachShader(pass.program, pass.pixelID);
			glLinkProgram(pass.program);
		}

		void TechniqueGL::GetHandlers(Pass_ &pass) {
			int number = 0;
			for (std::size_t i = 0; i < Parser.attributes.size(); i++) {
				int loc = glGetAttribLocation(pass.program, Parser.attributes[i].name.c_str());
				if (loc != -1) {
					shader::Shader_Var_ handler;
					handler = Parser.attributes[i];
					pass.handlers.insert(std::pair<shader::Shader_Var_, int>(handler, loc));
					number++;
				}
			}

			for (std::size_t i = 0; i < Parser.uniforms.size(); i++) {
				int loc = glGetUniformLocation(pass.program, Parser.uniforms[i].name.c_str());
				if (loc != -1) {
					shader::Shader_Var_ handler;
					handler = Parser.uniforms[i];
					pass.handlers.insert(std::pair<shader::Shader_Var_, int>(handler, loc));
					number++;
				}
			}		
		}



		void EffectGL::AddTechnique(CTechnique_* tech) {
		}

		void EffectGL::RemoveTechnique(std::string name) {

		}

		void EffectGL::SetBool(std::string handler, bool &) {

		}

		void EffectGL::SetInt(std::string handler, int &) {

		}

		void EffectGL::SetFloat(std::string handler, float &) {

		}

		void EffectGL::SetVec2(std::string handler, XVECTOR2 &) {

		}

		void EffectGL::SetVec3(std::string handler, XVECTOR3 &) {

		}

		void EffectGL::SetMat2(std::string handler, float*) {

		}

		void EffectGL::SetMat3(std::string handler, float*) {

		}

		void EffectGL::SetMat4(std::string handler, XMATRIX44 &) {

		}

		void EffectGL::SetTechnique(std::string name) {

		}

	}
}