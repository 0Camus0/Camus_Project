#include <WindowManager/AppManager.h>

#if defined(OS_WIN32)
#include <WindowManager/Win32App.h>
#elif defined(OS_ANDROID)
#include <WindowManager/AndroidApp.h>
#endif

#include <iostream>

#if !USE_C11_THREADS
void* AppManager::BridgeFunction(void *pctx) {
	GetAppManager().MainAppThread();
	return 0;
}
#endif


AppManager& GetAppManager() {
	static AppManager windows_manager;
	return windows_manager;
}

void AppManager::CreateApp() {
#if USE_C11_THREADS
	_thread = std::thread(&AppManager::MainAppThread,this);
#else
	pthread_create(&_thread, NULL, &AppManager::BridgeFunction, 0);
#endif
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

void AppManager::Join() {
#if USE_C11_THREADS
	_thread.join();
#else
	pthread_join(_thread,0);
#endif
}