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
/*
static void android_app_write_cmd(AndroidApp* pApp, int8_t cmd) {
	if (write(g_Msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
		LogPrintDebug("Failure writing android_app cmd: %s\n", strerror(errno));
	}
}
*/
int8_t android_app_read_cmd(AndroidApp* pApp) {
	int8_t cmd;
	if (read(g_Mgread, &cmd, sizeof(cmd)) == sizeof(cmd)) {
		switch (cmd) {
		case APP_CMD_SAVE_STATE:
			//free_saved_state
			break;
		}
		return cmd;
	}
	else {
		LogPrintDebug("No data on command pipe!");
	}
	return -1;
}

static void  ProcessCmd(AndroidApp* pApp, PollSource *source) {
	LogPrintDebug("ProcessCmd");
	int8_t cmd = android_app_read_cmd(pApp);
	switch (cmd)
	{
	case APP_CMD_RESUME:
	case APP_CMD_START:
	case APP_CMD_PAUSE:
	case APP_CMD_STOP: {
#ifdef USE_C11_THREADS
		LogPrintDebug("activityState=%d\n", cmd);
		g_mutex.lock();
		pApp->m_ActivityState = cmd;
		g_cond.notify_all();
		g_mutex.unlock();
#else
		pthread_mutex_lock(&g_mutex);
		pApp->m_ActivityState = cmd;
		pthread_cond_broadcast(&g_cond);
		pthread_mutex_unlock(&g_mutex);
#endif
		}break;
	}
}

static void  ProcessInput(AndroidApp* pApp, PollSource *source) {
	LogPrintDebug("ProcessInput");
	AInputEvent* event = NULL;
	while (AInputQueue_getEvent(pApp->m_pInputQueue, &event) >= 0) {
		LogPrintDebug("New input event: type=%d\n", AInputEvent_getType(event));
		if (AInputQueue_preDispatchEvent(pApp->m_pInputQueue, event)) {
			continue;
		}
		int32_t handled = 1;

		AInputQueue_finishEvent(pApp->m_pInputQueue, event, handled);
	}
}



// Called  from App Thread
AndroidApp::AndroidApp(){
	LogPrintDebug("AndroidApp::AndroidApp()");

	m_pConfig = 0;
	m_pInputQueue = 0;
	m_Looper = 0;
	m_ActivityState = -1;
	m_ActivityRaisedFlag = false;
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

	if (m_pConfig != 0) {
		AConfiguration_delete(m_pConfig);
		m_pConfig = 0;
	}

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
	LogPrintDebug("OnCreateApplication() ");

	m_cmdPoll.id  = LOOPER_ID_MAIN;
	m_cmdPoll.app = this;
	m_cmdPoll.process = ProcessCmd;

	m_inputPoll.id = LOOPER_ID_INPUT;
	m_inputPoll.app = this;
	m_inputPoll.process = ProcessInput;


	m_Looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	
	LogPrintDebug("OnCreateApplication() Val m_looper %d ", (int)m_Looper);

	int answer = ALooper_addFd(m_Looper, g_Mgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, 0 ,&m_cmdPoll);

	LogPrintDebug("OnCreateApplication() Val addFd %d", answer);
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
	
	int ident;
	int events;
	PollSource* source;

	if ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
		if (source != NULL) {
	//		LogPrintDebug("ALooper_pollAll() source != NULL");
			source->process(this, source);
		}
	}
	
	//usleep(16 * 1000);

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

// Called from Activity Thread
void AndroidApp::onDestroy(ANativeActivity* activity){
	LogPrintDebug("onDestroy");
	return; // Nothing to do for us since this may o may not be called, also when changin orientation it IS called, so, better leave it alone.
}
// Called from Activity Thread
void AndroidApp::onStart(ANativeActivity* activity){
	LogPrintDebug("onStart");
	
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_START;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_START;
	pthread_mutex_unlock(&g_mutex);
#endif
	
}
// Called from Activity Thread
void AndroidApp::onResume(ANativeActivity* activity){
	LogPrintDebug("onResume");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_RESUME;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_RESUME;
	pthread_mutex_unlock(&g_mutex);
#endif
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
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_PAUSE;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_PAUSE;
	pthread_mutex_unlock(&g_mutex);
#endif

	Suspend();
}
// Called from Activity Thread
void AndroidApp::onStop(ANativeActivity* activity){
	LogPrintDebug("onStop");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_STOP;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_STOP;
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidApp::onConfigurationChanged(ANativeActivity* activity){
	LogPrintDebug("onConfigurationChanged");

#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_CONFIG_CHANGED;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_CONFIG_CHANGED;
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidApp::onLowMemory(ANativeActivity* activity){
	LogPrintDebug("onLowMemory");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_LOW_MEMORY;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = APP_CMD_LOW_MEMORY;
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidApp::onWindowFocusChanged(ANativeActivity* activity, int focused){
	LogPrintDebug("onWindowFocusChanged");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_ActivityState = focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS;
	pthread_mutex_unlock(&g_mutex);
#endif
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
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_pInputQueue = queue;
	AInputQueue_attachLooper(_app->m_pInputQueue,_app->m_Looper, LOOPER_ID_INPUT, NULL,&_app->m_inputPoll);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_pInputQueue = queue;
	AInputQueue_attachLooper(_app->m_pInputQueue, _app->m_Looper, LOOPER_ID_INPUT, NULL, &_app->m_inputPoll);
	pthread_mutex_unlock(&g_mutex);
#endif
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue){
	LogPrintDebug("onInputQueueDestroyed");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidApp *_app = (AndroidApp*)(pApp);
	AInputQueue_detachLooper(_app->m_pInputQueue);
	_app->m_pInputQueue = 0;
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidApp *_app = (AndroidApp*)(pApp);
	_app->m_pInputQueue = 0;
	AInputQueue_detachLooper(_app->m_pInputQueue);
	pthread_mutex_unlock(&g_mutex);
#endif
}
