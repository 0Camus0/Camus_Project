#ifndef CA_APP_MANAGER_H
#define CA_APP_MANAGER_H

/*
#if __cplusplus > 199711L
#define USE_C11_THREADS
#endif
*/

#ifdef USE_C11_THREADS
#include <thread>
#include <mutex>
#include <condition_variable>
#else
#include <pthread.h>
#include <sched.h>
#endif

#include <Core/Core.h>
#include <memory>


#ifdef OS_ANDROID
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#endif

class FrameworkManager {
public:
	FrameworkManager() {}
	void	CreateApp(hyperspace::AppBase* pApp);
	void	MainAppThread();
	void	ResetApp();

	void	CreateAppThread();
	void	InitMutexAndVarConditions();
	void	Join();

	~FrameworkManager();
//#ifdef USE_C11_THREADS
	static void* BridgeFunction(void *pctx);
//#endif
};


#endif