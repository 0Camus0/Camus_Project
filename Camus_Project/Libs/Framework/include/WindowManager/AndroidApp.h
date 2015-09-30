#ifndef CA_ANDROID_APP_H
#define CA_ANDROID_APP_H

/*
#if __cplusplus > 199711L
#define USE_C11_THREADS
#endif
*/

#include <Core/Core.h>
#include <Driver/BaseDriver.h>
#include <memory>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>



class AndroidApp : public RootApp {
public:
	AndroidApp();
	void InitGlobalVars();
	void OnCreateApplication();
	void OnDestroyApplication();
	void OnInterruptApplication();
	void OnResumeApplication();
	void UpdateApplication();
	void ResetApplication();
	~AndroidApp();

	static void  onDestroy(ANativeActivity* activity);
	static void  onStart(ANativeActivity* activity);
	static void  onResume(ANativeActivity* activity);
	static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen);
	static void  onPause(ANativeActivity* activity);
	static void  onStop(ANativeActivity* activity);
	static void  onConfigurationChanged(ANativeActivity* activity);
	static void  onLowMemory(ANativeActivity* activity);
	static void  onWindowFocusChanged(ANativeActivity* activity, int focused);
	static void  onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window);
	static void  onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window);
	static void  onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue);
	static void  onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue);
	

	std::unique_ptr<BaseDriver>	m_pVideoDriver;
	

};


#endif