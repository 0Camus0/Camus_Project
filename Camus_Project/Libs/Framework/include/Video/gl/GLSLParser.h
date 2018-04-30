#ifndef T1000_GLSL_PARSER_H
#define T1000_GLSL_PARSER_H

#include <video/ShaderBase.h>

#include <string>
#include <vector>

namespace t1000 {
	namespace shader {
		struct GLSL_Var_ {
			semantic_	sem;
			datatype_	type;
			stage_		stage;
			std::string						name;
			int numItems;
		};

		class GLSL_Parser {
		public:
			GLSL_Parser();
			~GLSL_Parser();

			void ParseFromFile(std::string VSPath_, std::string FSPath_);
			void ParseFromMemory(std::string VSSrc_, std::string FSSrc_);

			std::vector<GLSL_Var_>	attributes;
			std::vector<GLSL_Var_>	varying;
			std::vector<GLSL_Var_>	uniforms;

		private:

			void Process(std::string &b);
			void ProcessToken(std::size_t &pos, std::vector<std::string> &v);
			void DetermineSemantic(GLSL_Var_ &var, std::string &str);
			void DetermineType(GLSL_Var_ &var, std::string &str);
			void DetermineArrayNum(GLSL_Var_ &var, std::string &str);
			std::string					buffer_vertex;
			std::string					buffer_fragment;
			stage_  current_stage;

		};
	}
}



#endif
