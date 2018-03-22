#ifndef GLSL_PARSER_H
#define GLSL_PARSER_H

#include  <Driver\ShaderBase.h>

#include <string>
#include <vector>

namespace hyperspace {
	namespace video {
		
		class GLSL_Parser {
		public:
			GLSL_Parser();
			~GLSL_Parser();

			void Parse(std::string Path_);

			std::vector<shader::Shader_Var_>	attributes;
			std::vector<shader::Shader_Var_>	varying;
			std::vector<shader::Shader_Var_>	uniforms;

		private:

			void Process(std::string &b);
			void ProcessToken(std::size_t &pos, std::vector<std::string> &v);
			void DetermineSemantic(shader::Shader_Var_ &var, std::string &str);
			void DetermineType(shader::Shader_Var_ &var, std::string &str);
			std::string					buffer_vertex;
			std::string					buffer_fragment;
			shader::stage_  current_stage;
		};
	}
}


#endif
