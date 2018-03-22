#include <WindowManager\FrameworkManager.h>
#include <Utils\Log.h>

#if defined(OS_WIN32)
#include <WindowManager\Win32Framework.h>
#elif defined(OS_ANDROID)
#include <WindowManager\AndroidFramework.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
int							g_Mgread;
int							g_Msgwrite;
#elif defined(__APPLE__)
#include <WindowManager\iOSFramework.h>
#endif

#include <iostream>

bool								g_bAppRunning = false; // Needs to be global, because it's shared by different threads but same class.
hyperspace::RootFramework			*pFramework = 0;
  

#ifdef USE_C11_THREADS
std::thread						g_thread;
std::mutex						g_mutex;
std::condition_variable			g_cond;
#else
pthread_t						g_thread;
pthread_mutex_t					g_mutex;
pthread_cond_t					g_cond;

#endif
void* FrameworkManager::BridgeFunction(void *pctx) {
	((FrameworkManager*)pctx)->MainAppThread();
	return 0;
}

void FrameworkManager::CreateApp(hyperspace::AppBase* pApp) {
	LogPrintDebug("CreateApp");

#if defined(OS_WIN32)
	pFramework = new Win32Framework(pApp);
#elif defined(OS_ANDROID)
	pFramework = new AndroidFramework(pApp);

	int msgpipe[2];
	if (pipe(msgpipe)) {
		LogPrintDebug("Could not create pipe: %s", strerror(errno));
		return;
	}
	g_Mgread = msgpipe[0];
	g_Msgwrite = msgpipe[1];
#elif defined(__APPLE__)
    pFramework = new iOSFramework(pApp);
#endif

	pApp->SetParentFramework(pFramework);

	InitMutexAndVarConditions();

	CreateAppThread();

}

void FrameworkManager::ResetApp() {
#ifdef USE_C11_THREADS
	g_mutex.lock();
#else
	pthread_mutex_lock(&g_mutex);
#endif
	pFramework->ResetApplication();
#ifdef USE_C11_THREADS
	g_mutex.unlock();
#else
	pthread_mutex_unlock(&g_mutex);
#endif
}

void FrameworkManager::CreateAppThread() {


		g_bAppRunning = false;

#ifdef USE_C11_THREADS
		g_thread = std::thread(&FrameworkManager::BridgeFunction, this);
#if defined(OS_ANDROID) || defined(__APPLE__)
		g_thread.detach();
#endif

		{
			std::unique_lock<std::mutex> locker(g_mutex);
			while (!g_bAppRunning) {
				g_cond.wait(locker);
			}
		}
#else
	#if defined(OS_ANDROID) || defined(__APPLE__)
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		LogPrintDebug("CreateAppThread pthread_create");
		pthread_create(&g_thread, &attr, &FrameworkManager::BridgeFunction, this);
	#else
		pthread_create(&g_thread, NULL, &FrameworkManager::BridgeFunction, this);
	#endif
    
#ifndef __APPLE__
        LogPrintDebug("Before semaphore");
		pthread_mutex_lock(&g_mutex);
		while (!g_bAppRunning) {
            LogPrintDebug("semaphore wait");
			pthread_cond_wait(&g_cond, &g_mutex);
		}
		pthread_mutex_unlock(&g_mutex);
        LogPrintDebug("After semaphore");
#endif

#endif
}

void FrameworkManager::MainAppThread() {

	LogPrintDebug("MainAppThread");
    
#ifdef __APPLE__
    LogPrintDebug("Before semaphore");
    pthread_mutex_lock(&g_mutex);
    while (!g_bAppRunning) {
        LogPrintDebug("semaphore wait");
        pthread_cond_wait(&g_cond, &g_mutex);
    }
    pthread_mutex_unlock(&g_mutex);
    LogPrintDebug("After semaphore");
#endif
    
	pFramework->InitGlobalVars();

	pFramework->OnCreateApplication();
    
#ifndef __APPLE__
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
#endif

	LogPrintDebug("MainAppThread start");

	while (g_bAppRunning) {
		pFramework->UpdateApplication();
	}
	LogPrintDebug("MainAppThread end");

	pFramework->OnDestroyApplication();

	
#ifdef USE_C11_THREADS
	g_mutex.lock();
#else
	pthread_mutex_lock(&g_mutex);
#endif
	LogPrintDebug("MainAppThread deleting app");
	delete pFramework;
	pFramework = 0;
#ifdef USE_C11_THREADS
	g_cond.notify_all();
	g_mutex.unlock();
#else
	pthread_cond_broadcast(&g_cond);
	pthread_mutex_unlock(&g_mutex);
#endif
}


void FrameworkManager::Join() {
#ifdef USE_C11_THREADS
	#ifdef OS_WIN32
		g_thread.join();
	#endif
#else
	pthread_join(g_thread,0);
#endif
}

FrameworkManager::~FrameworkManager(){
	LogPrintDebug("-~AppManager~~~~~~~~~~~~");
}

void FrameworkManager::InitMutexAndVarConditions() {
	LogPrintDebug("InitMutexAndVarConditions");
#ifndef USE_C11_THREADS
	pthread_mutex_init(&g_mutex, NULL);
	pthread_cond_init(&g_cond, NULL);
#endif
}
