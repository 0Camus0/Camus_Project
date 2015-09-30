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

extern std::unique_ptr<ANativeActivity>		g_pActivity;

// Called  from App Thread
AndroidApp::AndroidApp(){

	LogPrintDebug("[Thread app] - AndroidApp::AndroidApp()");

	ResetApplication();

}

AndroidApp::~AndroidApp() {
	LogPrintDebug("~~~~~~~~~AndroidApp::~AndroidApp()");
}

void AndroidApp::ResetApplication(){
	LogPrintDebug("[Thread app] - AndroidApp::SetCallbacks()");
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
}
// Called from App Thread
void AndroidApp::InitGlobalVars() {
	LogPrintDebug("[Thread app] - InitGlobalVars()");
}
// Called from App Thread
void AndroidApp::OnCreateApplication() {
	LogPrintDebug("[Thread app] - OnCreateApplication()");
}
// Called from App Thread
void AndroidApp::OnDestroyApplication() {
	LogPrintDebug("[Thread app] - OnDestroyApplication()");
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
	//LogPrintDebug("[Thread app] - UpdateApplication()");
	
	//usleep(5000 * 1000);
}
// Called from Activity Thread
void AndroidApp::onDestroy(ANativeActivity* activity){
	LogPrintInfo("[Thread Activity] -onDestroy");
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
}
// Called from Activity Thread
void AndroidApp::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window){
	LogPrintInfo("[Thread Activity] -onNativeWindowDestroyed");
}
// Called from Activity Thread
void AndroidApp::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("[Thread Activity] -onInputQueueCreated");
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("[Thread Activity] -onInputQueueDestroyed");
}