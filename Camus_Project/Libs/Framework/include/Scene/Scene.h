#ifndef CA_SCENE_H
#define CA_SCENE_H

#include <Driver\BaseDriver.h>
#include <Utils\InputBase.h>


namespace hyperspace {
	namespace scene {
		class CScene_ {
		public:
			CScene_() {
			
			}

			~CScene_() {
			
			}
			virtual void CreateAssets() = 0;
			virtual void DestroyAssets() = 0;

			virtual void OnUpdate(unsigned int dt) = 0;
			virtual void OnDraw() = 0;
			virtual void OnInput() = 0;

			virtual void OnPause() = 0;
			virtual void OnResume() = 0;

			void SetDriver(video::BaseDriver* pDriver){
				pVideoDriver = pDriver;
			}

			void SetEventManager(input::EventManager* pManager) {
				pEventManager = pManager;
			}

		private:
			input::EventManager		*pEventManager;
			video::BaseDriver		*pVideoDriver;
		};
	}
}

#endif