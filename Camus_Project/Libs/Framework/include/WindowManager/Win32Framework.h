#ifndef CA_WIN32_APP_H
#define CA_WIN32_APP_H

#include <Core/Core.h>
#include <Driver\BaseDriver.h>

#include <memory>

class Win32Framework : public t1000::RootFramework {
	public:
		Win32Framework(t1000::AppBase *pBaseApp) : RootFramework(pBaseApp) {
	
		}
		void InitGlobalVars();
		void OnCreateApplication();
		void OnDestroyApplication();
		void OnInterruptApplication();
		void OnResumeApplication();
		void UpdateApplication();
		void ProcessInput();
		void ResetApplication();
		~Win32Framework() {
		
		}
};


#endif