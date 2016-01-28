// TestApplication.cpp : Defines the entry point for the console application.
//

#include <WindowManager/FrameworkManager.h>
#include <iostream>
#include <memory.h>

#include "Test.h"

FrameworkManager					*pAppManager = 0;
TestApp								*pTestApp = 0;

#ifdef OS_ANDROID

#include <WindowManager/AndroidFramework.h>
#include <Utils/Log.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

ANativeActivity					*g_pActivity;

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
	
	g_pActivity = activity;

	if (pAppManager==0) {
		LogPrintDebug("ANativeActivity_onCreate: Activity Virgin");
		pAppManager = new FrameworkManager;
		pTestApp = new TestApp;
		pAppManager->CreateApp((hyperspace::AppBase*)pTestApp);
	}else{
		LogPrintDebug("ANativeActivity_onCreate: Activity Changed");
		pAppManager->ResetApp();
	}

}

#elif defined(OS_WIN32)
#include <stdio.h>
#include <tchar.h>

#include <Driver/RenderState.h>

int main()
{
	pAppManager = new FrameworkManager;
	pTestApp = new TestApp;

	pAppManager->CreateApp((hyperspace::AppBase*)pTestApp);

	std::cout << "Join..." << std::endl;
	pAppManager->Join();


	std::cout << "After Join, ready to close" << std::endl;



    return 0;
}

#elif defined(__APPLE__)

#include <Utils/Log.h>

#import <UIKit/UIKit.h>
#include <WindowManager/iOSFramework.h>


int main(int argc, char * argv[]) {
    pAppManager = new FrameworkManager;
    pTestApp = new TestApp;
    
    pAppManager->CreateApp((hyperspace::AppBase*)pTestApp);

    
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#endif

