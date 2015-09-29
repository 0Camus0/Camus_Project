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

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {

	pAppManager = std::unique_ptr<AppManager>(new AppManager);

	pAppManager->BridgeNativeAcitvity(activity, savedState, savedStateSize);

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