#include <WindowManager/AndroidApp.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>
#include <Driver/OpenGLDriver.h>

#include <unistd.h>

#ifdef USE_C11_THREADS
#include <thread>
#include <mutex>
#include <condition_variable>
extern std::mutex						g_mutex;
extern std::condition_variable			g_cond;
#else
#include <pthread.h>
#include <sched.h>
extern pthread_mutex_t					g_mutex;
extern pthread_cond_t					g_cond;
#endif

extern int		g_Mgread;
extern int		g_Msgwrite;
extern bool g_bAppRunning;

extern std::unique_ptr<ANativeActivity>		g_pActivity;
extern RootApp								*pApp;

void Suspend() {
	LogPrintDebug("Suspend APP");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	g_bAppRunning = false;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	g_bAppRunning = false;
	pthread_mutex_unlock(&g_mutex);
#endif
}

void Resume() {
	LogPrintDebug("Resume APP");
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
}

void CheckSuspend() {
#ifdef USE_C11_THREADS
	std::unique_lock<std::mutex> locker(g_mutex);
	while (!g_bAppRunning) {
		LogPrintDebug("CheckSuspend while waiting");
		g_cond.wait(locker);
	}
#else
	pthread_mutex_lock(&g_mutex);
	while (!g_bAppRunning) { 
		LogPrintDebug("CheckSuspend while waiting");
		pthread_cond_wait(&g_cond, &g_mutex); 
	}
	pthread_mutex_unlock(&g_mutex);
#endif
}

// Called  from App Thread
AndroidApp::AndroidApp(){

	LogPrintDebug("AndroidApp::AndroidApp()");

	ResetApplication();


}

AndroidApp::~AndroidApp() {
	LogPrintDebug("~AndroidApp::~AndroidApp()");
}

void AndroidApp::ResetApplication(){
	LogPrintDebug("AndroidApp::SetCallbacks()");
	g_pActivity->callbacks->onDestroy = onDestroy;
	g_pActivity->callbacks->onStart = onStart;
	g_pActivity->callbacks->onResume = onResume;
	g_pActivity->callbacks->onSaveInstanceState = onSaveInstanceState;
	g_pActivity->callbacks->onPause = onPause;
	g_pActivity->callbacks->onStop = onStop;
	g_pActivity->callbacks->onConfigurationChanged = onConfigurationChanged;
	g_pActivity->callbacks->onLowMemory = onLowMemory;
	g_pActivity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
	g_pActivity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
	g_pActivity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	g_pActivity->callbacks->onInputQueueCreated = onInputQueueCreated;
	g_pActivity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

	m_pConfig = AConfiguration_new();

	AConfiguration_fromAssetManager(m_pConfig, g_pActivity->assetManager);

	PrintCurrentConfig();
}
// Called from App Thread
void AndroidApp::InitGlobalVars() {
	LogPrintDebug("InitGlobalVars()");
}
// Called from App Thread
void AndroidApp::OnCreateApplication() {
	
	m_cmdPoll.m_Id  = LOOPER_ID_MAIN;
	m_cmdPoll.m_App = this;
	m_cmdPoll.process = AndroidApp::ProcessCmd;
	m_inputPoll.m_Id = LOOPER_ID_INPUT;
	m_inputPoll.m_App = this;
	m_inputPoll.process = AndroidApp::ProcessInput;

	LogPrintDebug("OnCreateApplication() 4");

	m_Looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);

	LogPrintDebug("OnCreateApplication() 5");

	ALooper_addFd(m_Looper,g_Msgwrite, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,&m_cmdPoll);

	LogPrintDebug("OnCreateApplication() 6");
}

// Called from App Thread
void AndroidApp::OnDestroyApplication() {
	LogPrintDebug("OnDestroyApplication()");
	AConfiguration_delete(m_pConfig);
}
// Called from App Thread
void AndroidApp::OnInterruptApplication() {
	LogPrintDebug("OnInterruptApplication()");
}
// Called from App Thread
void AndroidApp::OnResumeApplication() {
	LogPrintDebug("OnResumeApplication()");
}
// Called from App Thread
void AndroidApp::UpdateApplication() {
	//LogPrintDebug("UpdateApplication()");
		
	usleep(16 * 1000);

	CheckSuspend();
}



void AndroidApp::PrintCurrentConfig() {
	char lang[2], country[2];
	AConfiguration_getLanguage(m_pConfig, lang);
	AConfiguration_getCountry(m_pConfig, country);

	LogPrintDebug("Print Config: mcc=%d mnc=%d lang=%c%c cnt=%c%c orien=%d touch=%d dens=%d "
		"keys=%d nav=%d keysHid=%d navHid=%d sdk=%d size=%d long=%d "
		"modetype=%d modenight=%d",
		AConfiguration_getMcc(m_pConfig),
		AConfiguration_getMnc(m_pConfig),
		lang[0], lang[1], country[0], country[1],
		AConfiguration_getOrientation(m_pConfig),
		AConfiguration_getTouchscreen(m_pConfig),
		AConfiguration_getDensity(m_pConfig),
		AConfiguration_getKeyboard(m_pConfig),
		AConfiguration_getNavigation(m_pConfig),
		AConfiguration_getKeysHidden(m_pConfig),
		AConfiguration_getNavHidden(m_pConfig),
		AConfiguration_getSdkVersion(m_pConfig),
		AConfiguration_getScreenSize(m_pConfig),
		AConfiguration_getScreenLong(m_pConfig),
		AConfiguration_getUiModeType(m_pConfig),
		AConfiguration_getUiModeNight(m_pConfig));
}

void  AndroidApp::ProcessCmd(AndroidApp* pApp, PollSource *source) {
	LogPrintDebug("ProcessCmd");
}

void  AndroidApp::ProcessInput(AndroidApp* pApp, PollSource *source) {
	LogPrintDebug("ProcessInput");
}

// Called from Activity Thread
void AndroidApp::onDestroy(ANativeActivity* activity){
	LogPrintDebug("onDestroy");
#ifdef USE_C11_THREADS
	
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	AConfiguration_delete(_app->m_pConfig);
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidApp::onStart(ANativeActivity* activity){
	LogPrintDebug("onStart");
}
// Called from Activity Thread
void AndroidApp::onResume(ANativeActivity* activity){
	LogPrintDebug("onResume");
}
// Called from Activity Thread
void* AndroidApp::onSaveInstanceState(ANativeActivity* activity, size_t* outLen){
	LogPrintDebug("onSaveInstanceState");
	*outLen = 0;
	return 0;
}
// Called from Activity Thread
void AndroidApp::onPause(ANativeActivity* activity){
	LogPrintDebug("onPause");
	Suspend();
}
// Called from Activity Thread
void AndroidApp::onStop(ANativeActivity* activity){
	LogPrintDebug("onStop");
}
// Called from Activity Thread
void AndroidApp::onConfigurationChanged(ANativeActivity* activity){
	LogPrintDebug("onConfigurationChanged");
}
// Called from Activity Thread
void AndroidApp::onLowMemory(ANativeActivity* activity){
	LogPrintDebug("onLowMemory");
}
// Called from Activity Thread
void AndroidApp::onWindowFocusChanged(ANativeActivity* activity, int focused){
	LogPrintDebug("onWindowFocusChanged");
}
// Called from Activity Thread
void AndroidApp::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window){
	LogPrintDebug("onNativeWindowCreated");
	Resume();
}
// Called from Activity Thread
void AndroidApp::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window){
	LogPrintDebug("onNativeWindowDestroyed");
}
// Called from Activity Thread
void AndroidApp::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue){
	LogPrintDebug("onInputQueueCreated");
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue){
	LogPrintDebug("onInputQueueDestroyed");
}
