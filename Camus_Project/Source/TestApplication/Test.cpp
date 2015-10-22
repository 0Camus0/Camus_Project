#include "Test.h"
#include <Utils/Log.h>


void TestApp::CreateAssets() {
	LogPrintDebug("TestApp::CreateAssets");

	bInited = true;
}

void TestApp::DestroyAssets() {
	LogPrintDebug("TestApp::DestroyAssets");
}

void TestApp::OnUpdate(unsigned int dt) {

}

void TestApp::OnDraw() {

}

void TestApp::OnInput() {

}

void TestApp::OnPause() {
	LogPrintDebug("TestApp::OnPause");
}

void TestApp::OnResume() {
	LogPrintDebug("TestApp::OnResume");
}

void TestApp::OnReset() {
	LogPrintDebug("TestApp::OnReset()");
}