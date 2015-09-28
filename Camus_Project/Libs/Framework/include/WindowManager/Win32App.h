#ifndef CA_WIN32_APP_H
#define CA_WIN32_APP_H

#include <Core/Core.h>

class Win32App : public RootApp {
	public:
		void InitGlobalVars();
		void OnCreateApplication();
		void OnDestroyApplication();
		void OnInterruptApplication();
		void OnResumeApplication();
		void UpdateApplication();
};


#endif