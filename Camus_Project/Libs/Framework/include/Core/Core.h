#ifndef CA_CORE_H
#define CA_CORE_H

#include <Utils/InputBase.h>
#include <Driver/BaseDriver.h>

namespace hyperspace {
	class RootApp {
	public:
		virtual void InitGlobalVars() = 0;
		virtual void OnCreateApplication() = 0;
		virtual void OnDestroyApplication() = 0;
		virtual void OnInterruptApplication() = 0;
		virtual void OnResumeApplication() = 0;
		virtual void UpdateApplication() = 0;
		virtual void ProcessInput() = 0;
		virtual void ResetApplication() = 0;
		virtual ~RootApp() = 0;

		EventManager	*pEventManager;
		BaseDriver		*pVideoDriver;
	};
}

#endif