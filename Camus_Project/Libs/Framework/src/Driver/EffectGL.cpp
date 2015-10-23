#include <Driver/EffectGL.h>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

#include <GLES2/gl2.h>

#include <fstream>

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


			auto idVertex = CompileShader(shader::stage_::VERTEX_SHADER, (Path + std::string("_VS.glsl")), "");
			auto idPixel = CompileShader(shader::stage_::PIXEL_SHADER, (Path + std::string("_FS.glsl")), "");

			Pass_ pass0;
			pass0.args = "";
			pass0.name = "0";
			pass0.pVertexShader = new shader::VertexShaderGL(idVertex);
			pass0.pPixelShader = new shader::PixelShaderGL(idPixel);
			Passes.push_back(pass0);

		}

		void TechniqueGL::AddPass(Pass_ pass) {

		}

		void TechniqueGL::RemovePass(int id) {

		}

		std::int32_t TechniqueGL::GetNumPasses() {
			return 1;
		}

		void TechniqueGL::SetPass(int index) {

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

			LogPrintDebug("Compiling Shader %s ...", path.c_str());

			// LogPrintDebug("Content\n\n%s\n\n", strSource.c_str());

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