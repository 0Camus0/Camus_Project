#include <WindowManager/AndroidApp.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>
#include <Driver/OpenGLDriver.h>

AndroidApp::AndroidApp(ANativeActivity* activity, void* savedState, size_t savedStateSize) :
	RootApp(true)
	, m_pActivity(activity)
	, m_pvSavedState(savedState)
	, m_i_SavedStateSize(savedStateSize) {

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

void AndroidApp::InitGlobalVars() {

}

void AndroidApp::OnCreateApplication() {

}

void AndroidApp::OnDestroyApplication() {

}

void AndroidApp::OnInterruptApplication() {

}

void AndroidApp::OnResumeApplication() {

}

void AndroidApp::UpdateApplication() {

}

void AndroidApp::onDestroy(ANativeActivity* activity){
	LogPrintInfo("onDestroy");
}

void AndroidApp::onStart(ANativeActivity* activity){
	LogPrintInfo("onStart");
}

void AndroidApp::onResume(ANativeActivity* activity){
	LogPrintInfo("onResume");
}

void* AndroidApp::onSaveInstanceState(ANativeActivity* activity, size_t* outLen){
	LogPrintInfo("onSaveInstanceState");
	return 0;
}

void AndroidApp::onPause(ANativeActivity* activity){
	LogPrintInfo("onPause");
}

void AndroidApp::onStop(ANativeActivity* activity){
	LogPrintInfo("onStop");
}

void AndroidApp::onConfigurationChanged(ANativeActivity* activity){
	LogPrintInfo("onConfigurationChanged");
}

void AndroidApp::onLowMemory(ANativeActivity* activity){
	LogPrintInfo("onLowMemory");
}

void AndroidApp::onWindowFocusChanged(ANativeActivity* activity, int focused){
	LogPrintInfo("onWindowFocusChanged");
}

void AndroidApp::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window){
	LogPrintInfo("onNativeWindowCreated");
}

void AndroidApp::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window){
	LogPrintInfo("onNativeWindowDestroyed");
}

void AndroidApp::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("onInputQueueCreated");
}

void AndroidApp::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue){
	LogPrintInfo("onInputQueueDestroyed");
}