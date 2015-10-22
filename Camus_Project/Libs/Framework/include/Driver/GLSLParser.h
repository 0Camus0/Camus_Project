#ifndef GLSL_PARSER_H
#define GLSL_PARSER_H

#include  <Driver/ShaderBase.h>

#include <string>
#include <vector>

namespace hyperspace {
	namespace video {
		struct GLSL_Var_ {
			shader::semantic_	sem;
			shader::datatype_	type;
			shader::stage_		stage;
			std::string			name;
		};

		class GLSL_Parser {
		public:
			GLSL_Parser();
			~GLSL_Parser();

			void Parse(std::string Path_);

			std::vector<GLSL_Var_>	attributes;
			std::vector<GLSL_Var_>	varying;
			std::vector<GLSL_Var_>	uniforms;

		private:

			void Process(std::string &b);
			void ProcessToken(std::size_t &pos, std::vector<std::string> &v);
			void DetermineSemantic(GLSL_Var_ &var, std::string &str);
			void DetermineType(GLSL_Var_ &var, std::string &str);
			std::string					buffer_vertex;
			std::string					buffer_fragment;
			shader::stage_  current_stage;
		};
	}
}


#endif
