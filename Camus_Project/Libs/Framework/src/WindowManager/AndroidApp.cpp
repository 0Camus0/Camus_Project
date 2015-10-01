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

/*
void  ProcessCmd(AndroidApp* pApp, PollSource *source) {
	LogPrintInfo("[Thread Activity] -ProcessCmd");
}

void  ProcessInput(AndroidApp* pApp, PollSource *source) {
	LogPrintInfo("[Thread Activity] -ProcessInput");
}

// Called from Activity Thread
void onDestroy(ANativeActivity* activity) {
	LogPrintInfo("[Thread Activity] -onDestroy");
}
// Called from Activity Thread
void onStart(ANativeActivity* activity) {
	LogPrintInfo("[Thread Activity] -onStart");
}
// Called from Activity Thread
void onResume(ANativeActivity* activity) {
	LogPrintInfo("[Thread Activity] -onResume");
}
// Called from Activity Thread
void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
	LogPrintInfo("[Thread Activity] -onSaveInstanceState");
	*outLen = 0;
	return 0;
}
// Called from Activity Thread
void onPause(ANativeActivity* activity) {
	LogPrintInfo("[Thread Activity] -onPause");
}
// Called from Activity Thread
void onStop(ANativeActivity* activity) {
	LogPrintInfo("[Thread Activity] -onStop");
}
// Called from Activity Thread
void onConfigurationChanged(ANativeActivity* activity) {
	LogPrintInfo("[Thread Activity] -onConfigurationChanged");
}
// Called from Activity Thread
void onLowMemory(ANativeActivity* activity) {
	LogPrintInfo("[Thread Activity] -onLowMemory");
}
// Called from Activity Thread
void onWindowFocusChanged(ANativeActivity* activity, int focused) {
	LogPrintInfo("[Thread Activity] -onWindowFocusChanged");
}
// Called from Activity Thread
void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
	LogPrintInfo("[Thread Activity] -onNativeWindowCreated");
}
// Called from Activity Thread
void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
	LogPrintInfo("[Thread Activity] -onNativeWindowDestroyed");
}
// Called from Activity Thread
void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
	LogPrintInfo("[Thread Activity] -onInputQueueCreated");
}

void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
	LogPrintInfo("[Thread Activity] -onInputQueueDestroyed");
}
*/

void Suspend() {
	LogPrintInfo("[Thread Activity] -Suspend APP");
	pthread_mutex_lock(&g_mutex);
	g_bAppRunning = false;
	pthread_mutex_unlock(&g_mutex);
}

void Resume() {
	LogPrintInfo("[Thread Activity] -Resume APP");
	pthread_mutex_lock(&g_mutex);
	g_bAppRunning = true;
	pthread_cond_broadcast(&g_cond);
	pthread_mutex_unlock(&g_mutex);
}

void CheckSuspend() {
	pthread_mutex_lock(&g_mutex);
	while (!g_bAppRunning) { 
		LogPrintInfo("[Thread Activity] -CheckSuspend while waiting");
		pthread_cond_wait(&g_cond, &g_mutex); 
	}
	pthread_mutex_unlock(&g_mutex);
}

// Called  from App Thread
AndroidApp::AndroidApp(){

	LogPrintDebug("[Thread app] - AndroidApp::AndroidApp()");

	ResetApplication();

	m_RaiseChange = false;

}

AndroidApp::~AndroidApp() {
	LogPrintDebug("~~~~~~~~~AndroidApp::~AndroidApp()");
}

void AndroidApp::ResetApplication(){
	LogPrintDebug("[XXX app] - AndroidApp::SetCallbacks()");
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
	LogPrintDebug("[Thread app] - InitGlobalVars()");
}
// Called from App Thread
void AndroidApp::OnCreateApplication() {
	
	m_cmdPoll.m_Id  = LOOPER_ID_MAIN;
	m_cmdPoll.m_App = this;
	m_cmdPoll.process = ProcessCmd;
	m_inputPoll.m_Id = LOOPER_ID_INPUT;
	m_inputPoll.m_App = this;
	m_inputPoll.process = ProcessInput;

	LogPrintDebug("[Thread app] - OnCreateApplication() 4");

	m_Looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);

	LogPrintDebug("[Thread app] - OnCreateApplication() 5");

	ALooper_addFd(m_Looper,g_Msgwrite, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,&m_cmdPoll);

	LogPrintDebug("[Thread app] - OnCreateApplication() 6");
}

// Called from App Thread
void AndroidApp::OnDestroyApplication() {
	LogPrintDebug("[Thread app] - OnDestroyApplication()");
	AConfiguration_delete(m_pConfig);
}
// Called from App Thread
void AndroidApp::OnInterruptApplication() {
	LogPrintDebug("[Thread app] - OnInterruptApplication()");
}
// Called from App Thread
void AndroidApp::OnResumeApplication() {
	LogPrintDebug("[Thread app] - OnResumeApplication()");
}
// Called from App Thread
void AndroidApp::UpdateApplication() {
	LogPrintDebug("[Thread app] - UpdateApplication()");
		
	usleep(16 * 1000);

	CheckSuspend();
}



void AndroidApp::PrintCurrentConfig() {
	char lang[2], country[2];
	AConfiguration_getLanguage(m_pConfig, lang);
	AConfiguration_getCountry(m_pConfig, country);

	LogPrintInfo("[Thread app] - Print Config: mcc=%d mnc=%d lang=%c%c cnt=%c%c orien=%d touch=%d dens=%d "
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
	LogPrintInfo("[Thread Activity] -ProcessCmd");
}

void  AndroidApp::ProcessInput(AndroidApp* pApp, PollSource *source) {
	LogPrintInfo("[Thread Activity] -ProcessInput");
}

// Called from Activity Thread
void AndroidApp::onDestroy(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onDestroy");
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	AConfiguration_delete(_app->m_pConfig);
	pthread_mutex_unlock(&g_mutex);	
}
// Called from Activity Thread
void AndroidApp::onStart(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onStart");
}
// Called from Activity Thread
void AndroidApp::onResume(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onResume");
}
// Called from Activity Thread
void* AndroidApp::onSaveInstanceState(ANativeActivity* activity, size_t* outLen){
	LogPrintInfo("[Thread Activity] -onSaveInstanceState");
	*outLen = 0;
	return 0;
}
// Called from Activity Thread
void AndroidApp::onPause(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onPause");
}
// Called from Activity Thread
void AndroidApp::onStop(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onStop");
}
// Called from Activity Thread
void AndroidApp::onConfigurationChanged(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onConfigurationChanged");
}
// Called from Activity Thread
void AndroidApp::onLowMemory(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onLowMemory");
}
// Called from Activity Thread
void AndroidApp::onWindowFocusChanged(ANativeActivity* activity, int focused){
	LogPrintInfo("[Thread Activity] -onWindowFocusChanged");
}
// Called from Activity Thread
void AndroidApp::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window){
	LogPrintInfo("[Thread Activity] -onNativeWindowCreated");
	Resume();
}
// Called from Activity Thread
void AndroidApp::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window){
	LogPrintInfo("[Thread Activity] -onNativeWindowDestroyed");
	Suspend();
}
// Called from Activity Thread
void AndroidApp::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("[Thread Activity] -onInputQueueCreated");
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("[Thread Activity] -onInputQueueDestroyed");
}
