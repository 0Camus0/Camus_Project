#ifndef CA_ANDROID_APP_H
#define CA_ANDROID_APP_H

#include <Core/Core.h>
#include <Driver/BaseDriver.h>
#include <memory>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

class AndroidApp : public RootApp {
public:
	AndroidApp() :
		RootApp(true) {}
	void InitGlobalVars();
	void OnCreateApplication();
	void OnDestroyApplication();
	void OnInterruptApplication();
	void OnResumeApplication();
	void UpdateApplication();

	static void onDestroy(ANativeActivity* activity);
	static void onStart(ANativeActivity* activity);
	static void onResume(ANativeActivity* activity);
	static void onSaveInstanceState(ANativeActivity* activity);
	static void onPause(ANativeActivity* activity);
	static void onStop(ANativeActivity* activity);
	static void onConfigurationChanged(ANativeActivity* activity);
	static void onLowMemory(ANativeActivity* activity);
	static void onWindowFocusChanged(ANativeActivity* activity);
	static void onNativeWindowCreated(ANativeActivity* activity);
	static void onNativeWindowDestroyed(ANativeActivity* activity);
	static void onInputQueueCreated(ANativeActivity* activity);
	static void onInputQueueDestroyed(ANativeActivity* activity);


	std::unique_ptr<BaseDriver>	m_pVideoDriver;
};


#endif