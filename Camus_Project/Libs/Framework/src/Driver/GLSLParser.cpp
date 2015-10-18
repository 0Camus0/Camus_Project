#include <Driver/GLSLParser.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

namespace hyperspace{
	namespace video {

		GLSL_Parser::GLSL_Parser(std::string Path_) {
			{
				std::ifstream ifs((Path_ + std::string(".vert")).c_str());
				if (ifs.good()) {
					std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
					buffer_vertex = str;
				}
				ifs.close();
			}

			{
				std::ifstream ifs((Path_ + std::string(".frag")).c_str());
				if (ifs.good()) {
					std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
					buffer_fragment = str;
				}
				ifs.close();
			}
		}

		GLSL_Parser::~GLSL_Parser() {
		}

		void GLSL_Parser::Parse() {
			current_stage = shader::stage_::VERTEX_SHADER;
			Process(buffer_vertex);
			current_stage = shader::stage_::PIXEL_SHADER;
			Process(buffer_fragment);
		}

		void GLSL_Parser::Process(std::string &b) {
			std::cout << "\n\n\n" << std::endl;

			std::istringstream iss(b);
			std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>{} };

			int pos = 0;
			for (std::size_t i = 0; i < tokens.size(); i++) {

				if ((pos = tokens[i].find("uniform")) != std::string::npos) {
					ProcessToken(i, tokens);
				}

				if ((pos = tokens[i].find("varying")) != std::string::npos) {
					ProcessToken(i, tokens);
				}

				if ((pos = tokens[i].find("attribute")) != std::string::npos) {
					ProcessToken(i, tokens);
				}
			}
		}

		void GLSL_Parser::ProcessToken(std::size_t &pos, std::vector<std::string> &v) {
			int token_pos = pos;
			for (;;) {
				if (v[token_pos].find(";") != std::string::npos) break;
				token_pos++;
			}

			GLSL_Var_ var_;
			var_.name = v[token_pos].substr(0, v[token_pos].size() - 1);
			var_.stage = current_stage;
			DetermineSemantic(var_, v[pos]);
			DetermineType(var_, v[token_pos - 1]);

			switch (var_.sem) {
			case shader::semantic_::ATTRIBUTE: {
				attributes.push_back(var_);
			}break;
			case shader::semantic_::UNIFORM: {
				uniforms.push_back(var_);
			}break;
			case shader::semantic_::VARYING: {
				varying.push_back(var_);
			}break;
			case shader::semantic_::UNKNOWN_SEMANTIC: {
			}break;
			}

		}

		void GLSL_Parser::DetermineSemantic(GLSL_Var_ &var, std::string &str) {
			if (str.find("uniform") != std::string::npos) {
				var.sem = shader::semantic_::UNIFORM;
			}
			else if (str.find("varying") != std::string::npos) {
				var.sem = shader::semantic_::VARYING;
			}
			else if (str.find("attribute") != std::string::npos) {
				var.sem = shader::semantic_::ATTRIBUTE;
			}
			else {
				var.sem = shader::semantic_::UNKNOWN_SEMANTIC;
			}
		}

		void GLSL_Parser::DetermineType(GLSL_Var_ &var, std::string &str) {
			if (str.find("int") != std::string::npos) {
				var.type = shader::datatype_::INT_;
			}
			else if (str.find("float") != std::string::npos) {
				var.type = shader::datatype_::FLOAT_;
			}
			else if (str.find("bool") != std::string::npos) {
				var.type = shader::datatype_::BOOLEAN_;
			}
			else if (str.find("vec2") != std::string::npos) {
				var.type = shader::datatype_::VECTOR2_;
			}
			else if (str.find("vec3") != std::string::npos) {
				var.type = shader::datatype_::VECTOR3_;
			}
			else if (str.find("vec4") != std::string::npos) {
				var.type = shader::datatype_::VECTOR4_;
			}
			else if (str.find("mat2") != std::string::npos) {
				var.type = shader::datatype_::MAT2_;
			}
			else if (str.find("mat3") != std::string::npos) {
				var.type = shader::datatype_::MAT3_;
			}
			else if (str.find("mat4") != std::string::npos) {
				var.type = shader::datatype_::MAT4_;
			}
			else if (str.find("sampler1D") != std::string::npos) {
				var.type = shader::datatype_::SAMPLER1D_;
			}
			else if (str.find("sampler2D") != std::string::npos) {
				var.type = shader::datatype_::SAMPLER2D_;
			}
			else if (str.find("sampler3D") != std::string::npos) {
				var.type = shader::datatype_::SAMPLER3D_;
			}
			else if (str.find("samplerCube?") != std::string::npos) {
				var.type = shader::datatype_::SAMPLERCUBE_;
			}
			else if (str.find("sampler2DShadow?") != std::string::npos) {
				var.type = shader::datatype_::SAMPLERSHADOW_;
			}
			else {
				var.type = shader::datatype_::UNKNOWN_TYPE;
			}
		}
	}

}