#ifndef CA_CORE_H
#define CA_CORE_H

#include <Utils/InputBase.h>
#include <Driver/BaseDriver.h>

namespace hyperspace {

	class AppBase {
	public:
		AppBase() : bInited(false) {}
		virtual void CreateAssets() = 0;
		virtual void DestroyAssets() = 0;

		virtual void OnUpdate(unsigned int dt) = 0;
		virtual void OnDraw() = 0;
		virtual void OnInput() = 0;

		virtual void OnPause() = 0;
		virtual void OnResume() = 0;

		virtual void OnReset() = 0;

		bool bInited;
	};

	class RootFramework {
	public:
		RootFramework(AppBase *pApp) : pBaseApp(pApp) {}
		virtual void InitGlobalVars() = 0;
		virtual void OnCreateApplication() = 0;
		virtual void OnDestroyApplication() = 0;
		virtual void OnInterruptApplication() = 0;
		virtual void OnResumeApplication() = 0;
		virtual void UpdateApplication() = 0;
		virtual void ProcessInput() = 0;
		virtual void ResetApplication() = 0;
		virtual ~RootFramework() = 0;
	

		input::EventManager		*pEventManager;
		video::BaseDriver		*pVideoDriver;
		AppBase					*pBaseApp;
	};
}

#endif