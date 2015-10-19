// TestApplication.cpp : Defines the entry point for the console application.
//

#include <WindowManager/AppManager.h>
#include <iostream>
#include <memory.h>

AppManager					*pAppManager = 0;

#ifdef OS_ANDROID

#include <WindowManager/AndroidApp.h>
#include <Utils/Log.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

ANativeActivity					*g_pActivity;

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
	
	g_pActivity = activity;

	if (pAppManager==0) {
		LogPrintDebug("ANativeActivity_onCreate: Activity Virgin");
		pAppManager = new AppManager;
		pAppManager->CreateApp();
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
	pAppManager = new AppManager;

	pAppManager->CreateApp();

	hyperspace::video::CRenderState a, b;
	b.draworder = hyperspace::video::draw_order_::CW;

	std::cout << "iguales: " << (int)(a == b) << std::endl;

	std::cout << "Join..." << std::endl;



	pAppManager->Join();


	std::cout << "After Join, ready to close" << std::endl;



    return 0;
}

#endif