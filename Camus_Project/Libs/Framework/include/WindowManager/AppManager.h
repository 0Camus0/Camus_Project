#ifndef CA_APP_MANAGER_H
#define CA_APP_MANAGER_H

#if __cplusplus > 199711L
#define USE_C11_THREADS 0
#else
#define USE_C11_THREADS 0
#endif

#include <Core/Core.h>
#include <memory>

#if USE_C11_THREADS
#include <thread>
#else
#include <pthread.h>
#endif

#ifdef OS_ANDROID
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#endif

class AppManager {
public:
	std::unique_ptr<RootApp>	pApp;

	void	CreateApp();
	void	MainAppThread();
	void	Join();

#if USE_C11_THREADS
	std::thread	_thread;
#else
	pthread_t	_thread;
	static void* BridgeFunction(void *pctx);
#endif

};

AppManager&	GetAppManager();

#endif