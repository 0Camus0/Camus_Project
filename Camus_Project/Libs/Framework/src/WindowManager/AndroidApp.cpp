#include <WindowManager/AndroidApp.h>
#include <Utils/WindowProperties.h>
#include <Driver/OpenGLDriver.h>

AndroidApp::AndroidApp(ANativeActivity* activity, void* savedState, size_t savedStateSize) :
	RootApp(true)
	, m_pActivity(activity)
	, m_pvSavedState(savedState)
	, m_i_SavedStateSize(savedStateSize) {

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

}

void AndroidApp::onStart(ANativeActivity* activity){

}

void AndroidApp::onResume(ANativeActivity* activity){

}

void AndroidApp::onSaveInstanceState(ANativeActivity* activity){

}

void AndroidApp::onPause(ANativeActivity* activity){

}

void AndroidApp::onStop(ANativeActivity* activity){

}

void AndroidApp::onConfigurationChanged(ANativeActivity* activity){

}

void AndroidApp::onLowMemory(ANativeActivity* activity){

}

void AndroidApp::onWindowFocusChanged(ANativeActivity* activity){

}

void AndroidApp::onNativeWindowCreated(ANativeActivity* activity){

}

void AndroidApp::onNativeWindowDestroyed(ANativeActivity* activity){

}

void AndroidApp::onInputQueueCreated(ANativeActivity* activity){

}

void AndroidApp::onInputQueueDestroyed(ANativeActivity* activity){

}