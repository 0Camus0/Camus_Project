#include <WindowManager/AppManager.h>

#if defined(OS_WIN32)
#include <WindowManager/Win32App.h>
#elif defined(OS_ANDROID)
#include <WindowManager/AndroidApp.h>
#endif

AppManager& AppManager::getinstance() {
	static AppManager windows_manager;
	return windows_manager;
}

void AppManager::CreateApp() {
#if defined(OS_WIN32)
	pApp = new Win32App();
#elif defined(OS_ANDROID)

#endif

	



}