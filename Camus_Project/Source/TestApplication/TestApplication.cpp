// TestApplication.cpp : Defines the entry point for the console application.
//

#include <WindowManager/AppManager.h>
#include <iostream>
#include <memory.h>

std::unique_ptr<AppManager>							pAppManager;

#ifdef OS_ANDROID

#include <WindowManager/AndroidApp.h>
#include <Utils/Log.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

std::unique_ptr<ANativeActivity>					g_pActivity;

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
	LogPrintDebug("[Thread Activity] - ANativeActivity_onCreate");
	
	g_pActivity.reset(activity);
	static ANativeActivity* old_activity = activity;

	if (old_activity == activity) {
		LogPrintDebug("[Thread Activity] - ANativeActivity_onCreate: Activity Virgin");
		pAppManager.reset(new AppManager);
		pAppManager->CreateApp();
	}else{
		LogPrintDebug("[Thread Activity] - ANativeActivity_onCreate: Activity Changed");
		pAppManager->ResetApp();
	}

	old_activity = activity;
}

#elif defined(OS_WIN32)
#include <stdio.h>
#include <tchar.h>


int main()
{
	pAppManager.reset(new AppManager);

	pAppManager->CreateApp();

	std::cout << "Join..." << std::endl;

	pAppManager->Join();

	std::cout << "After Join, ready to close" << std::endl;


    return 0;
}

#endif