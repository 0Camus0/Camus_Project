#include <Driver/GLSLParser.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

namespace hyperspace{
	namespace video {

		GLSL_Parser::GLSL_Parser() {
		}

		GLSL_Parser::~GLSL_Parser() {
		}

		void GLSL_Parser::Parse(std::string Path_) {
			{
				std::ifstream ifs((Path_ + std::string("_VS.glsl")).c_str());
				if (ifs.good()) {
					std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
					buffer_vertex = str;
				}
				ifs.close();
			}

			{
				std::ifstream ifs((Path_ + std::string("_FS.glsl")).c_str());
				if (ifs.good()) {
					std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
					buffer_fragment = str;
				}
				ifs.close();
			}

			current_stage = shader::stage_::VERTEX_SHADER;
			Process(buffer_vertex);
			current_stage = shader::stage_::PIXEL_SHADER;
			Process(buffer_fragment);
		}

		void GLSL_Parser::Process(std::string &b) {

			std::istringstream iss(b);
			std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>{} };

			std::size_t pos = 0;
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
			int token_pos = static_cast<int>(pos);
			for (;;) {
				if (v[token_pos].find(";") != std::string::npos) break;
				token_pos++;
			}

			shader::Shader_Var_ var_;
			var_.name = v[token_pos].substr(0, v[token_pos].size() - 1);
			var_.name[var_.name.size()] = 0;
			var_.props |= current_stage;
			DetermineSemantic(var_, v[pos]);
			DetermineType(var_, v[token_pos - 1]);

			if(var_.props & shader::semantic_::ATTRIBUTE) {
				attributes.push_back(var_);
			}else if(var_.props & shader::semantic_::UNIFORM) {
				uniforms.push_back(var_);
			}else if (var_.props & shader::semantic_::VARYING) {
				varying.push_back(var_);
			}
		}

		void GLSL_Parser::DetermineSemantic(shader::Shader_Var_ &var, std::string &str) {
			if (str.find("uniform") != std::string::npos) {
				var.props |= shader::semantic_::UNIFORM;
			}
			else if (str.find("varying") != std::string::npos) {
				var.props |= shader::semantic_::VARYING;
			}
			else if (str.find("attribute") != std::string::npos) {
				var.props |= shader::semantic_::ATTRIBUTE;
			}
		}

		void GLSL_Parser::DetermineType(shader::Shader_Var_ &var, std::string &str) {
			if (str.find("int") != std::string::npos) {
				var.props |= shader::datatype_::INT_;
			}
			else if (str.find("float") != std::string::npos) {
				var.props |= shader::datatype_::FLOAT_;
			}
			else if (str.find("bool") != std::string::npos) {
				var.props |= shader::datatype_::BOOLEAN_;
			}
			else if (str.find("vec2") != std::string::npos) {
				var.props |= shader::datatype_::VECTOR2_;
			}
			else if (str.find("vec3") != std::string::npos) {
				var.props |= shader::datatype_::VECTOR3_;
			}
			else if (str.find("vec4") != std::string::npos) {
				var.props |= shader::datatype_::VECTOR4_;
			}
			else if (str.find("mat2") != std::string::npos) {
				var.props |= shader::datatype_::MAT2_;
			}
			else if (str.find("mat3") != std::string::npos) {
				var.props |= shader::datatype_::MAT3_;
			}
			else if (str.find("mat4") != std::string::npos) {
				var.props |= shader::datatype_::MAT4_;
			}
			else if (str.find("sampler1D") != std::string::npos) {
				var.props |= shader::datatype_::SAMPLER1D_;
			}
			else if (str.find("sampler2D") != std::string::npos) {
				var.props |= shader::datatype_::SAMPLER2D_;
			}
			else if (str.find("sampler3D") != std::string::npos) {
				var.props |= shader::datatype_::SAMPLER3D_;
			}
			else if (str.find("samplerCube?") != std::string::npos) {
				var.props |= shader::datatype_::SAMPLERCUBE_;
			}
			else if (str.find("sampler2DShadow?") != std::string::npos) {
				var.props |= shader::datatype_::SAMPLERSHADOW_;
			}
		}
	}

}