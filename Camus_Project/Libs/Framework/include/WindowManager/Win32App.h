#ifndef CA_WIN32_APP_H
#define CA_WIN32_APP_H

#include <Core/Core.h>
#include <Driver/BaseDriver.h>
#include <memory>

class Win32App : public hyperspace::RootApp {
	public:
		Win32App() {}
		void InitGlobalVars();
		void OnCreateApplication();
		void OnDestroyApplication();
		void OnInterruptApplication();
		void OnResumeApplication();
		void UpdateApplication();
		void ProcessInput();
		void ResetApplication();
};


#endif