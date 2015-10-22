#include <Driver/EffectGL.h>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

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

			for (std::size_t i = 0; i < Parser.attributes.size(); i++) {
				LogPrintDebug("Attribute: %s ", Parser.attributes[i].name.c_str());
			}
		
			for (std::size_t i = 0; i < Parser.uniforms.size(); i++) {
				LogPrintDebug("Uniform: %s ", Parser.uniforms[i].name.c_str());
			}

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