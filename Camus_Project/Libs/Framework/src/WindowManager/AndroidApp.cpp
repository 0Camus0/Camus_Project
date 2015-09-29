#include <WindowManager/AndroidApp.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>
#include <Driver/OpenGLDriver.h>

extern ANativeActivity*						g_pActivity;
extern void*								g_pvSavedState;
extern size_t								g_i_SavedStateSize;

// Called from Activity Thread
AndroidApp::AndroidApp() :
	RootApp(true) {

	m_pActivity = g_pActivity;
	m_pvSavedState = g_pvSavedState;
	m_i_SavedStateSize = g_i_SavedStateSize;

	m_pActivity->callbacks->onDestroy = onDestroy;
	m_pActivity->callbacks->onStart = onStart;
	m_pActivity->callbacks->onResume = onResume;
	m_pActivity->callbacks->onSaveInstanceState = onSaveInstanceState;
	m_pActivity->callbacks->onPause = onPause;
	m_pActivity->callbacks->onStop = onStop;
	m_pActivity->callbacks->onConfigurationChanged = onConfigurationChanged;
	m_pActivity->callbacks->onLowMemory = onLowMemory;
	m_pActivity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
	m_pActivity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
	m_pActivity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	m_pActivity->callbacks->onInputQueueCreated = onInputQueueCreated;
	m_pActivity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;


}
// Called from App Thread
void AndroidApp::InitGlobalVars() {

}
// Called from App Thread
void AndroidApp::OnCreateApplication() {

}
// Called from App Thread
void AndroidApp::OnDestroyApplication() {

}
// Called from App Thread
void AndroidApp::OnInterruptApplication() {

}
// Called from App Thread
void AndroidApp::OnResumeApplication() {

}
// Called from App Thread
void AndroidApp::UpdateApplication() {
	LogPrintInfo("UpdateApplication");
}
// Called from Activity Thread
void AndroidApp::onDestroy(ANativeActivity* activity){
	LogPrintInfo("onDestroy");
}
// Called from Activity Thread
void AndroidApp::onStart(ANativeActivity* activity){
	LogPrintInfo("onStart");
}
// Called from Activity Thread
void AndroidApp::onResume(ANativeActivity* activity){
	LogPrintInfo("onResume");
}
// Called from Activity Thread
void* AndroidApp::onSaveInstanceState(ANativeActivity* activity, size_t* outLen){
	LogPrintInfo("onSaveInstanceState");
	return 0;
}
// Called from Activity Thread
void AndroidApp::onPause(ANativeActivity* activity){
	LogPrintInfo("onPause");
}
// Called from Activity Thread
void AndroidApp::onStop(ANativeActivity* activity){
	LogPrintInfo("onStop");
}
// Called from Activity Thread
void AndroidApp::onConfigurationChanged(ANativeActivity* activity){
	LogPrintInfo("onConfigurationChanged");
}
// Called from Activity Thread
void AndroidApp::onLowMemory(ANativeActivity* activity){
	LogPrintInfo("onLowMemory");
}
// Called from Activity Thread
void AndroidApp::onWindowFocusChanged(ANativeActivity* activity, int focused){
	LogPrintInfo("onWindowFocusChanged");
}
// Called from Activity Thread
void AndroidApp::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window){
	LogPrintInfo("onNativeWindowCreated");
}
// Called from Activity Thread
void AndroidApp::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window){
	LogPrintInfo("onNativeWindowDestroyed");
}
// Called from Activity Thread
void AndroidApp::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("onInputQueueCreated");
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("onInputQueueDestroyed");
}