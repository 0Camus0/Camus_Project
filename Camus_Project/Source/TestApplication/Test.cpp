#include "Test.h"
#include <Utils\Log.h>
#include <Utils\Time.h>

#ifdef OS_WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#else
#include <unistd.h>
#endif

#include <algorithm>
#include <iostream>
#include <cmath>


float clip(float n, float lower, float upper) {
	return std::max(
		lower,
		std::min(n, upper));
}
 
void TestApp::InitVars() {
	Cam.InitPerspective(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(46.8f), (float)pFramework->pVideoDriver->width / (float)pFramework->pVideoDriver->height, 2.0f, 12000.0f);
	Cam.Speed = 10.0f;
	Cam.Eye = XVECTOR3(0.0f, 9.75f, -31.0f);
	Cam.Pitch = 0.14f;
	Cam.Roll = 0.0f;
	Cam.Yaw = 0.020f;
	Cam.Update(0.0f);

	LightCam.InitOrtho(XVECTOR3(0.0f, 100.0f, 10.0f), 130.0f, 130.0f, 10.0f, 600.0f);
	LightCam.Speed = 10.0f;
	LightCam.Eye = XVECTOR3(25.0f, 100.0f, 0.0f);
	LightCam.Pitch = 1.12f;
	LightCam.Roll = 0.0f;
	LightCam.Yaw = -0.9f;
	LightCam.Update(0.0f);

	ActiveCam = &Cam;

	SceneProp.AddCamera(ActiveCam);
	SceneProp.AddLightCamera(&LightCam);

	SceneProp.AddLight(LightCam.Eye, XVECTOR3(1, 1, 1), 30000, true);
	SceneProp.AddLight(XVECTOR3(-55, 10, 0), XVECTOR3(1.0, 0.57, 0.16), 60, true);
	SceneProp.AddLight(XVECTOR3(55, 10, 0), XVECTOR3(1.0, 0.57, 0.16), 60, true);
	SceneProp.AddLight(XVECTOR3(60, 10, 30), XVECTOR3(1.0, 0.57, 0.16), 60, true);
	SceneProp.AddLight(XVECTOR3(60, 10, -30), XVECTOR3(1.0, 0.57, 0.16), 60, true);
	SceneProp.ActiveLights = 5;
	SceneProp.AmbientColor = XVECTOR3(0.8f, 0.8f, 0.8f);
}

void TestApp::LoadAssets() {

}

void TestApp::CreateAssets() {
	LogPrintDebug("TestApp::CreateAssets\n");

	PrimitiveMgr.Init();
	PrimitiveMgr.SetVP(&VP);


	int index = PrimitiveMgr.CreateMesh("Scene.X");


	Meshes[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);


	Meshes[0].TranslateAbsolute(0.0, -10.0f, 0.0f);
	Meshes[0].Update();

	PrimitiveMgr.SetSceneProps(&SceneProp);
	

 	bInited = true;
}

void TestApp::DestroyAssets() {
	LogPrintDebug("TestApp::DestroyAssets");
}

void TestApp::OnUpdate() {

	DtTimer.Update();
	DtSecs = DtTimer.GetDTSecs();


	

	ActiveCam->MoveYaw(0.015);


	ActiveCam->Update(DtSecs);
	VP = ActiveCam->VP;

	
	


}

void TestApp::OnDraw() {

	pFramework->pVideoDriver->Clear();

	Meshes[0].SetGlobalSignature(t1000::T_Signature::FORWARD_PASS);
	Meshes[0].Draw();

	pFramework->pVideoDriver->SwapBuffers();

	
}

void TestApp::OnInput() {

}

void TestApp::OnPause() {
	LogPrintDebug("TestApp::OnPause");
	bPaused = true;
}

void TestApp::OnResume() {
	LogPrintDebug("TestApp::OnResume");
	bPaused = false;
}

void TestApp::OnReset() {
	LogPrintDebug("TestApp::OnReset()");

	Cam.InitPerspective(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(46.8f),(float)pFramework->pVideoDriver->width / (float)pFramework->pVideoDriver->height, 2.0f, 12000.0f);
}

void TestApp::LoadScene(int id) {

}