#include <WindowManager/AppManager.h>
#include <Utils/Log.h>

#if defined(OS_WIN32)
#include <WindowManager/Win32App.h>
#elif defined(OS_ANDROID)
#include <WindowManager/AndroidApp.h>
#include <unistd.h>
#include <errno.h>
int							g_Mgread;
int							g_Msgwrite;
#endif

#include <iostream>

bool						g_bAppRunning = false; // Needs to be global, because it's shared by different threads but same class.
RootApp						*pApp = 0;
  

#ifdef USE_C11_THREADS
std::thread						g_thread;
std::mutex						g_mutex;
std::condition_variable			g_cond;
#else
pthread_t						g_thread;
pthread_mutex_t					g_mutex;
pthread_cond_t					g_cond;

#endif
void* AppManager::BridgeFunction(void *pctx) {
	((AppManager*)pctx)->MainAppThread();
	return 0;
}

void AppManager::CreateApp() {
	LogPrintDebug("CreateApp");

	InitMutexAndVarConditions();

	CreateAppThread();

}

void AppManager::ResetApp() {
#ifdef USE_C11_THREADS
	g_mutex.lock();
#else
	pthread_mutex_lock(&g_mutex);
#endif
	pApp->ResetApplication();
#ifdef USE_C11_THREADS
	g_mutex.unlock();
#else
	pthread_mutex_unlock(&g_mutex);
#endif
}

void AppManager::CreateAppThread() {

#if defined(OS_WIN32)
		pApp = new Win32App;
#elif defined(OS_ANDROID)
		pApp = new AndroidApp;

		int msgpipe[2];
		if (pipe(msgpipe)) {
			LogPrintDebug("Could not create pipe: %s", strerror(errno));
			return;
		}
		g_Mgread = msgpipe[0];
		g_Msgwrite = msgpipe[1];
#endif

		g_bAppRunning = false;

#ifdef USE_C11_THREADS
		g_thread = std::thread(&AppManager::BridgeFunction, this);
#ifdef OS_ANDROID
		g_thread.detach();
#endif

		{
			std::unique_lock<std::mutex> locker(g_mutex);
			while (!g_bAppRunning) {
				g_cond.wait(locker);
			}
		}
#else
	#ifdef OS_ANDROID
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		LogPrintDebug("CreateAppThread pthread_create");
		pthread_create(&g_thread, &attr, &AppManager::BridgeFunction, this);
	#else
		pthread_create(&g_thread, NULL, &AppManager::BridgeFunction, this);
	#endif
		pthread_mutex_lock(&g_mutex);
		while (!g_bAppRunning) {
			pthread_cond_wait(&g_cond, &g_mutex);
		}
		pthread_mutex_unlock(&g_mutex);

#endif
}

void AppManager::MainAppThread() {

	LogPrintDebug("MainAppThread");

	pApp->InitGlobalVars();

	pApp->OnCreateApplication();

#ifdef USE_C11_THREADS
	g_mutex.lock();
	g_bAppRunning = true;
	g_cond.notify_all();
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	g_bAppRunning = true;
	pthread_cond_broadcast(&g_cond);
	pthread_mutex_unlock(&g_mutex);
#endif

	while (g_bAppRunning) {
		pApp->UpdateApplication();
	}

	pApp->OnDestroyApplication();

}


void AppManager::Join() {
#ifdef USE_C11_THREADS
	#ifdef OS_WIN32
		g_thread.join();
	#endif
#else
	pthread_join(g_thread,0);
#endif
}

AppManager::~AppManager(){
	LogPrintDebug("-~AppManager~~~~~~~~~~~~");
}

void AppManager::InitMutexAndVarConditions() {
	LogPrintDebug("InitMutexAndVarConditions");
#ifndef USE_C11_THREADS
	pthread_mutex_init(&g_mutex, NULL);
	pthread_cond_init(&g_cond, NULL);
#endif
}
