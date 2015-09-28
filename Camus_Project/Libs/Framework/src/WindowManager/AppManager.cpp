#include <WindowManager/AppManager.h>

#if defined(OS_WIN32)
#include <WindowManager/Win32App.h>
#elif defined(OS_ANDROID)
#include <WindowManager/AndroidApp.h>
#endif

#include <thread>
#include <iostream>


AppManager& GetAppManager() {
	static AppManager windows_manager;
	return windows_manager;
}

void AppManager::CreateApp() {


//	std::thread _thread(&AppManager::MainAppThread,this);
//	_thread.detach();

	MainAppThread();

}

void AppManager::MainAppThread() {

#if defined(OS_WIN32)
	pApp = std::unique_ptr<RootApp>(new Win32App);
#elif defined(OS_ANDROID)
	//pApp = new AndoidApp();
#endif

	pApp->InitGlobalVars();

	pApp->OnCreateApplication();

	while (pApp->m_bAlive) {
		pApp->UpdateApplication();
	}

	pApp->OnDestroyApplication();

}