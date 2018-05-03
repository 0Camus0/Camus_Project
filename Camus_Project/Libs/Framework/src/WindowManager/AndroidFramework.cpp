#include <Config.h>

#ifdef OS_ANDROID


#include <WindowManager/AndroidFramework.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>
#include <video/BaseDriver.h>
#include <video/gl/GLDriver.h>
#include <Utils/FileSystem.h>

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

extern ANativeActivity						*g_pActivity;
extern t1000::RootFramework			*pFramework;

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

void CheckSuspend(AndroidFramework* pApp) {
#ifdef USE_C11_THREADS
	std::unique_lock<std::mutex> locker(g_mutex);
	while (!g_bAppRunning) {
		pApp->OnInterruptApplication();
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
int8_t android_app_read_cmd(AndroidFramework* pApp) {
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

void  _ProcessCmd(AndroidFramework* pApp, PollSource *source) {
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
		pApp->m_ActivityStateQueue.push_back(cmd);
		g_cond.notify_all();
		g_mutex.unlock();
#else
		pthread_mutex_lock(&g_mutex);
		pApp->m_ActivityStateQueue.push_back(cmd);
		pthread_cond_broadcast(&g_cond);
		pthread_mutex_unlock(&g_mutex);
#endif
		}break;
	}
}

void  _ProcessInput(AndroidFramework* pApp, PollSource *source) {
	pApp->ProcessInput();
}



// Called  from App Thread
AndroidFramework::AndroidFramework(t1000::AppBase *pBaseApp) :  RootFramework(pBaseApp){
	LogPrintDebug("AndroidApp::AndroidApp()");
	m_pConfig = 0;
	m_pInputQueue = 0;
	m_pWindow = 0;
	m_Looper = 0;
	m_ActivityStateQueue.clear();
	m_ActivityRaisedFlag = false;
	ResetApplication();
}

AndroidFramework::~AndroidFramework() {
	LogPrintDebug("~AndroidApp::~AndroidApp()");
	ANativeActivity_finish(g_pActivity);
}

void AndroidFramework::ResetApplication(){
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

	this->m_internalDataPath = std::string(g_pActivity->internalDataPath);
	this->m_externalDataPath = std::string(g_pActivity->externalDataPath);

	JNIEnv* env = g_pActivity->env;
	jclass clazz = env->GetObjectClass(g_pActivity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "getPackageCodePath", "()Ljava/lang/String;");
	jobject result = env->CallObjectMethod(g_pActivity->clazz, methodID);

	jboolean isCopy;
	std::string res = env->GetStringUTFChars((jstring)result, &isCopy);
	LogPrintDebug("Looked up package code path: %s", res.c_str());
	this->m_apkPath = res;
}
// Called from App Thread
void AndroidFramework::InitGlobalVars() {
	LogPrintDebug("InitGlobalVars()");
}
// Called from App Thread
void AndroidFramework::OnCreateApplication(t1000::ApplicationDesc desc) {
	LogPrintDebug("OnCreateApplication() ");

	t1000::fs::Filesystem::instance()->InitFS(m_internalDataPath,m_externalDataPath,m_apkPath);


	m_cmdPoll.id  = LOOPER_ID_MAIN;
	m_cmdPoll.app = this;
	m_cmdPoll.process = _ProcessCmd;

	m_inputPoll.id = LOOPER_ID_INPUT;
	m_inputPoll.app = this;
	m_inputPoll.process = _ProcessInput;

	m_Looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	
	ALooper_addFd(m_Looper, g_Mgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, 0 ,&m_cmdPoll);

	pEventManager = new t1000::input::EventManager();
	pEventManager->InitTouchScreen(AConfiguration_getTouchscreen(m_pConfig));


	pVideoDriver = new t1000::GLDriver();



	pVideoDriver->InitDriver();


}

void AndroidFramework::ChangeAPI(t1000::T_GRAPHICS_API::E api) {

}

void AndroidFramework::CheckState(){
	std::vector<int> currentStates;
#ifdef USE_C11_THREADS
	g_mutex.lock();
	currentStates = m_ActivityStateQueue;
	m_ActivityStateQueue.clear();
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	currentStates = m_ActivityStateQueue;
	m_ActivityStateQueue.clear();
	pthread_mutex_unlock(&g_mutex);
#endif

	for (std::size_t i = 0; i < currentStates.size(); i++){
		switch (currentStates[i]) {
			case APP_CMD_INIT_WINDOW: {
				pVideoDriver->SetWindow((void*)m_pWindow);
				pVideoDriver->InitDriver();
				if (pBaseApp->bInited) {
					pBaseApp->OnReset();
					pBaseApp->OnResume();
				}
				else {
					pBaseApp->InitVars();
					pBaseApp->CreateAssets();
				}
			}break;
			case APP_CMD_CONFIG_CHANGED:{
			/*	pVideoDriver->DestroySurfaces();
				pVideoDriver->SetWindow((void*)m_pWindow);
				pVideoDriver->InitDriver();
				pBaseApp->OnReset();
				pBaseApp->OnResume();*/
			}break;
			case APP_CMD_TERM_WINDOW: {
				pVideoDriver->DestroySurfaces();
			}break;
		}
	}
	
}

// Called from App Thread
void AndroidFramework::OnDestroyApplication() {
	LogPrintDebug("OnDestroyApplication()");

	pBaseApp->DestroyAssets();

	AInputQueue_detachLooper(m_pInputQueue);
	AConfiguration_delete(m_pConfig);
}
// Called from App Thread
void AndroidFramework::OnInterruptApplication() {
	LogPrintDebug("OnInterruptApplication()");
	pBaseApp->OnPause();
}
// Called from App Thread
void AndroidFramework::OnResumeApplication() {
	LogPrintDebug("OnResumeApplication()");
}
// Called from App Thread
void AndroidFramework::UpdateApplication() {
	//LogPrintDebug("UpdateApplication()");
	
	CheckState();

	int ident;
	int events;
	PollSource* source;

	if ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
		if (source != NULL) {
			source->process(this, source);
		}
	}

	pEventManager->queue.clear();

	if (!pBaseApp->bPaused && pBaseApp->bInited) {
		pVideoDriver->Update();
		pBaseApp->OnUpdate();
		pBaseApp->OnDraw();
	}


	CheckSuspend(this);
}

void AndroidFramework::ProcessInput() {
	while (AInputQueue_hasEvents(m_pInputQueue) == 1) {
		AInputEvent* event = NULL;
		AInputQueue_getEvent(m_pInputQueue, &event);

		if (AInputQueue_preDispatchEvent(m_pInputQueue, event)) {
			AInputQueue_finishEvent(m_pInputQueue, event, 1);
			continue;
		}

		int type = AInputEvent_getType(event);
		if (type == AINPUT_EVENT_TYPE_KEY) {
			int action = AKeyEvent_getAction(event);

			switch (action) {
				case AKEY_EVENT_ACTION_DOWN: {
					t1000::input::InputEvent_ tmp;
					int key = AKeyEvent_getKeyCode(event);
					int count = AKeyEvent_getRepeatCount(event);
					LogPrintDebug("Pressed key [%d] counter [%d]\n", key, count);
					tmp._state = t1000::input::TypeEvent_::KEY_PRESSED;
					pEventManager->queue.push_back(tmp);
				}break;

				case AKEY_EVENT_ACTION_UP: {
					t1000::input::InputEvent_ tmp;
					int key = AKeyEvent_getKeyCode(event);
					int count = AKeyEvent_getRepeatCount(event);
					LogPrintDebug("Released key [%d] counter [%d]\n", key, count);
					tmp._state = t1000::input::TypeEvent_::KEY_RELEASED;
					pEventManager->queue.push_back(tmp);
				}break;

				case AKEY_EVENT_ACTION_MULTIPLE: {
					LogPrintDebug("MULTIPLE\n");
				}break;
			}
		}
		else if (type == AINPUT_EVENT_TYPE_MOTION) {
			int action = AMotionEvent_getAction(event);
			unsigned int Flag = action & AMOTION_EVENT_ACTION_MASK;

			switch (Flag) {
				case AMOTION_EVENT_ACTION_DOWN:
				case AMOTION_EVENT_ACTION_POINTER_DOWN: {
					t1000::input::InputEvent_ tmp;
					tmp._time = AMotionEvent_getEventTime(event);
					tmp.fcoords[0] = AMotionEvent_getRawX(event, 0);
					tmp.fcoords[1] = AMotionEvent_getRawY(event, 0);
					tmp._state = t1000::input::TypeEvent_::TOUCH_PRESSED;
					pBaseApp->IManager.Touch.state[0] = true;
					pBaseApp->IManager.Touch.xCoord = (int)tmp.fcoords[0];
					pBaseApp->IManager.Touch.yCoord = (int)tmp.fcoords[1];
				//	LogPrintDebug("AMOTION_EVENT_ACTION_POINTER_DOWN [%f] [%f]\n", tmp.fcoords[0], tmp.fcoords[1]);
					
				}break;

				case AMOTION_EVENT_ACTION_UP:
				case AMOTION_EVENT_ACTION_POINTER_UP: {
					t1000::input::InputEvent_ tmp;
					tmp._time = AMotionEvent_getEventTime(event);
					tmp.fcoords[0] = AMotionEvent_getRawX(event, 0);
					tmp.fcoords[1] = AMotionEvent_getRawY(event, 0);
					tmp._state = t1000::input::TypeEvent_::TOUCH_PRESSED;
					pBaseApp->IManager.Touch.state[0] = false;
					pBaseApp->IManager.Touch.state[1] = false;
					pBaseApp->IManager.Touch.xCoord = (int)tmp.fcoords[0];
					pBaseApp->IManager.Touch.yCoord = (int)tmp.fcoords[1];
				//	LogPrintDebug("AMOTION_EVENT_ACTION_POINTER_UP [%f] [%f]\n", tmp.fcoords[0], tmp.fcoords[1]);
				}break;
				
				default: {
				//	LogPrintDebug("TOUCH MOVED\n");
				}break;
			}
		}

		int32_t handled = 1;
		AInputQueue_finishEvent(m_pInputQueue, event, handled);
	}
}

void AndroidFramework::PrintCurrentConfig() {
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

	LogPrintDebug("Internal Path[%s]\n", g_pActivity->internalDataPath);
	LogPrintDebug("External Path[%s]\n", g_pActivity->externalDataPath);
}

// Called from Activity Thread
void AndroidFramework::onDestroy(ANativeActivity* activity){
	LogPrintDebug("onDestroy");
	return; // Nothing to do for us since this may o may not be called, also when changing orientation it IS called, so, better leave it alone.
}
// Called from Activity Thread
void AndroidFramework::onStart(ANativeActivity* activity){
	LogPrintDebug("onStart");
	
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_START);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_START);
	pthread_mutex_unlock(&g_mutex);
#endif
	
}
// Called from Activity Thread
void AndroidFramework::onResume(ANativeActivity* activity){
	LogPrintDebug("onResume");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_RESUME);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_RESUME);
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void* AndroidFramework::onSaveInstanceState(ANativeActivity* activity, size_t* outLen){
	LogPrintDebug("onSaveInstanceState");
	*outLen = 0;
	return 0;
}
// Called from Activity Thread
void AndroidFramework::onPause(ANativeActivity* activity){
	LogPrintDebug("onPause");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_PAUSE);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_PAUSE);
	pthread_mutex_unlock(&g_mutex);
#endif

	Suspend();
}
// Called from Activity Thread
void AndroidFramework::onStop(ANativeActivity* activity){
	LogPrintDebug("onStop");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_STOP);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_STOP);
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidFramework::onConfigurationChanged(ANativeActivity* activity){
	LogPrintDebug("onConfigurationChanged");

#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_CONFIG_CHANGED);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_CONFIG_CHANGED);

	int Width = ANativeWindow_getWidth(_app->m_pWindow);
	int Height = ANativeWindow_getHeight(_app->m_pWindow);

	LogPrintDebug("onConfigurationChanged to [%d]x[%d]", Width, Height);

	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidFramework::onLowMemory(ANativeActivity* activity){
	LogPrintDebug("onLowMemory");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_LOW_MEMORY);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		_app->m_ActivityStateQueue.push_back(APP_CMD_LOW_MEMORY);
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidFramework::onWindowFocusChanged(ANativeActivity* activity, int focused){
	LogPrintDebug("onWindowFocusChanged");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		focused ? _app->m_ActivityStateQueue.push_back(APP_CMD_GAINED_FOCUS) : _app->m_ActivityStateQueue.push_back(APP_CMD_LOST_FOCUS);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if(_app)
		focused ? _app->m_ActivityStateQueue.push_back(APP_CMD_GAINED_FOCUS) : _app->m_ActivityStateQueue.push_back(APP_CMD_LOST_FOCUS);
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidFramework::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window){
	LogPrintDebug("onNativeWindowCreated");

#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app) {
		_app->m_pWindow = window;
		_app->m_ActivityStateQueue.push_back(APP_CMD_INIT_WINDOW);
	}
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app) {
		_app->m_pWindow = window;
		_app->m_ActivityStateQueue.push_back(APP_CMD_INIT_WINDOW);
	}
	pthread_mutex_unlock(&g_mutex);
#endif

	Resume();
}
// Called from Activity Thread
void AndroidFramework::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window){
	LogPrintDebug("onNativeWindowDestroyed");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app) {
		_app->m_pWindow = 0;
		_app->m_ActivityStateQueue.push_back(APP_CMD_TERM_WINDOW);
	}
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app) {
		_app->m_pWindow = 0;
		_app->m_ActivityStateQueue.push_back(APP_CMD_TERM_WINDOW);
	}
	pthread_mutex_unlock(&g_mutex);
#endif
}
// Called from Activity Thread
void AndroidFramework::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue){
	LogPrintDebug("onInputQueueCreated");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	_app->m_pInputQueue = queue;
	AInputQueue_attachLooper(_app->m_pInputQueue,_app->m_Looper, LOOPER_ID_INPUT, NULL,&_app->m_inputPoll);
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	_app->m_pInputQueue = queue;
	AInputQueue_attachLooper(_app->m_pInputQueue, _app->m_Looper, LOOPER_ID_INPUT, NULL, &_app->m_inputPoll);
	pthread_mutex_unlock(&g_mutex);
#endif
}

void AndroidFramework::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
	LogPrintDebug("onInputQueueDestroyed");
#ifdef USE_C11_THREADS
	g_mutex.lock();
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app) {
		AInputQueue_detachLooper(_app->m_pInputQueue);
		_app->m_pInputQueue = 0;
	}
	g_mutex.unlock();
#else
	pthread_mutex_lock(&g_mutex);
	AndroidFramework *_app = (AndroidFramework*)(pFramework);
	if (_app) {
		AInputQueue_detachLooper(_app->m_pInputQueue);
		_app->m_pInputQueue = 0;
	}
	pthread_mutex_unlock(&g_mutex);
#endif
}

#endif
