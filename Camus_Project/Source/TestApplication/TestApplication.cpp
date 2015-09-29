// TestApplication.cpp : Defines the entry point for the console application.
//

#include <WindowManager/AppManager.h>
#include <iostream>
#include <memory.h>

std::unique_ptr<AppManager> pAppManager;

#ifdef OS_ANDROID

#include <WindowManager/AndroidApp.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

ANativeActivity*					g_pActivity = 0;
void*								g_pvSavedState = 0;
size_t								g_i_SavedStateSize = 0;

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
	g_pActivity = activity;
	g_pvSavedState = savedState;
	g_i_SavedStateSize = savedStateSize;

	pAppManager = std::unique_ptr<AppManager>(new AppManager);

	pAppManager->CreateApp();
}

#elif defined(OS_WIN32)
#include <stdio.h>
#include <tchar.h>


int main()
{
	pAppManager = std::unique_ptr<AppManager>(new AppManager);

	pAppManager->CreateApp();

	std::cout << "Join..." << std::endl;

	pAppManager->Join();

	std::cout << "After Join, ready to close" << std::endl;


    return 0;
}

#endif