#include <Driver/EffectGL.h>


namespace hyperspace {
	namespace video {

		void TechniqueGL::Initialize(std::string name, std::string shader) {

		}

		void TechniqueGL::AddPass(Pass_ pass) {

		}

		void TechniqueGL::RemovePass(int id) {

		}

		std::int32_t TechniqueGL::GetNumPasses() {

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