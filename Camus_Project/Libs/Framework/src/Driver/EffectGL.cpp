#include <Driver/EffectGL.h>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <fstream>

namespace hyperspace {
	namespace video {

		void TechniqueGL::Initialize(std::string name, std::string shader, ArgumentsManager args) {
#if USE_LOG_DEBUG_TECHNIQUES
			LogPrintDebug("[TechniqueGL::Initialize] %s  shader: %s ", name.c_str(), shader.c_str());
#endif
			Name = name;
			Shader = shader;
			std::string Path = fs::Filesystem::instance()->GetResourcesPath();
			Path += "Shaders/";
			Path += shader;

			Parser.Parse(Path);
			AddPass("0", args , Path, CRenderStateDesc());

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
			return static_cast<int>(Passes.size());
		}

		void TechniqueGL::SetPass(std::size_t id) {
			CurrentPass = &Passes[0];
			glUseProgram(CurrentPass->program);
		}

		bool TechniqueGL::BindAttribute(std::string name, unsigned int pass, unsigned int binding){
			if (pass >= Passes.size())
				return false;

			Handlers::iterator it = Passes[pass].handlers.find(name);

			if(it!=Passes[pass].handlers.end()){
				it->second.props |= binding;
			}
            
            return true;
		}

		void TechniqueGL::DebugPassesContent(){
			for(std::size_t i=0;i<Passes.size();i++){
				for(Handlers::iterator it = Passes[i].handlers.begin(); it != Passes[i].handlers.end();it++){
					if (!(it->second.props & shader::semantic_::ATTRIBUTE))
						continue;

					std::string tmp_bind = "";
					if(it->second.props & shader::bind_::POSITION)
						tmp_bind  = "POSITION";
					if (it->second.props & shader::bind_::NORMAL)
						tmp_bind = "NORMAL";
					if (it->second.props & shader::bind_::TEXCOORD_0)
						tmp_bind = "TEXCOORD_1";
					if (it->second.props & shader::bind_::TEXCOORD_1)
						tmp_bind = "TEXCOORD_2";
					if (it->second.props & shader::bind_::TEXCOORD_2)
						tmp_bind = "TEXCOORD_3";
					if (it->second.props & shader::bind_::TANGENT)
						tmp_bind = "TANGENT";
					if (it->second.props & shader::bind_::BINORMAL)
						tmp_bind = "BINORMAL";
					if (it->second.props & shader::bind_::BONE_WEIGHTS)
						tmp_bind = "BONE_WEIGHTS";
					if (it->second.props & shader::bind_::BONE_INDICES)
						tmp_bind = "BONE_INDICES";

					LogPrintDebug("Name [%s] Binding [%s]",it->first.c_str(), tmp_bind.c_str());
				}
			}
		}

		unsigned int TechniqueGL::CompileShader(shader::stage_ type_, std::string path, std::string args) {
			GLuint type = GL_VERTEX_SHADER;
			
			if (type_ == shader::stage_::VERTEX_SHADER)
				type = GL_VERTEX_SHADER;
			
			if (type_ == shader::stage_::PIXEL_SHADER)
				type = GL_FRAGMENT_SHADER;

			std::ifstream in_(path.c_str(), std::ios::in| std::ios::binary);

			if (!in_.good()) {
				LogPrintError("[TechniqueGL] Shader on Path: %s not found.", path.c_str());
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
#if USE_LOG_DEBUG_TECHNIQUES
			LogPrintDebug("[TechniqueGL] Compiling shader [%s] with args [%s] ...", path.c_str(),args.c_str());
#endif

#if USE_LOG_DEBUG_SHADERS_CONTENT
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
					LogPrintError("[TechniqueGL] Failed to compile vertex shader: %s with args: %s\nLog[%s]", path.c_str(), args.c_str(), strError.c_str());
				if (type == GL_FRAGMENT_SHADER)
					LogPrintError("[TechniqueGL] Failed to compile pixel shader: %s with args: %s\nLog[%s]", path.c_str(), args.c_str(), strError.c_str());
				delete [] pError;				
			}
			else {
				LogPrintDebug("[TechniqueGL] Shader [%s] compiled. \n\n",path.c_str());
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
					pass.handlers.insert(std::pair<std::string, shader::Shader_Var_>(Parser.attributes[i].name, Parser.attributes[i]));
					number++;
				}
			}

			for (std::size_t i = 0; i < Parser.uniforms.size(); i++) {
				int loc = glGetUniformLocation(pass.program, Parser.uniforms[i].name.c_str());
				if (loc != -1) {
					pass.handlers.insert(std::pair<std::string, shader::Shader_Var_>(Parser.uniforms[i].name, Parser.uniforms[i]));
					number++;
				}
			}		
		}


		void TechniqueGL::SetBool(std::string handler, bool &b) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniform1i(it->second.loc, static_cast<int>(b) );
			}
		}

		void TechniqueGL::SetInt(std::string handler, int &i) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniform1i(it->second.loc, i);
			}
		}

		void TechniqueGL::SetFloat(std::string handler, float &f) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniform1f(it->second.loc, f);
			}
		}

		void TechniqueGL::SetVec2(std::string handler, XVECTOR2 &v) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniform2fv(it->second.loc,1,v);
			}
		}

		void TechniqueGL::SetVec3(std::string handler, XVECTOR3 &v) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniform3fv(it->second.loc, 1, v);
			}
		}

		void TechniqueGL::SetVec4(std::string handler, XVECTOR3 &v) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniform4fv(it->second.loc, 1, v);
			}
		}

		void TechniqueGL::SetMat2(std::string handler, float*m) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniformMatrix2fv(it->second.loc, 1, GL_FALSE, m);
			}
		}

		void TechniqueGL::SetMat3(std::string handler, float*m) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniformMatrix3fv(it->second.loc, 1, GL_FALSE, m);
			}
		}

		void TechniqueGL::SetMat4(std::string handler, XMATRIX44 &m) {
			Handlers::iterator it = CurrentPass->handlers.find(handler);
			if (it != CurrentPass->handlers.end()) {
				glUniformMatrix3fv(it->second.loc, 1, GL_FALSE, m);
			}
		}

	}
}