#include "Test.h"
#include <video\gl\GLDriver.h>
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

#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>

#define NUM_LIGHTS 1
#define RADI 170.0f

#define HIGHQ 1
#define MEDIUMQ 2
#define LOWQ 3

#define QUALITY_SELECTED LOWQ

#if   QUALITY_SELECTED == HIGHQ
#define MAX_QUALITY
#elif QUALITY_SELECTED == MEDIUMQ
#define MEDIUM_QUALITY
#elif QUALITY_SELECTED == LOWQ
#define LOW_QUALITY 
#endif

int RandomRange(int min_, int max_) {
	std::random_device rd;     
	std::mt19937 rng(rd());   
	std::uniform_int_distribution<int> uni(min_, max_);

	return uni(rng);;
}



float clip(float n, float lower, float upper) {
	return std::max(
		lower,
		std::min(n, upper));
}
 
void TestApp::InitVars() {
    Position = XVECTOR3(0.0f, 0.0f, 0.0f);
    Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
    Scaling = XVECTOR3(1.0f, 1.0f, 1.0f);
    SelectedMesh = 0;

    CamSelection = NORMAL_CAM1;
    SceneSettingSelection = CHANGE_EXPOSURE;
  
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


	ShadowFilter.kernelSize = 4;
	ShadowFilter.radius = 1.f;
	ShadowFilter.sigma = 1.0f;
	ShadowFilter.Update();

	BloomFilter.kernelSize = 11;
	BloomFilter.radius = 2.5f;
	BloomFilter.sigma = 4.5f;
	BloomFilter.Update();


	NearDOFFilter.kernelSize = 8;
	NearDOFFilter.radius = 2.5f;
	NearDOFFilter.sigma = 4.5f;
	NearDOFFilter.Update();

	SceneProp.Aperture = 120;
	SceneProp.FocalLength = 50;
	SceneProp.MaxCoc = 2.5;
#ifdef  MAX_QUALITY
	SceneProp.DOF_Near_Samples_squared = 1.0f;
	SceneProp.DOF_Far_Samples_squared = 3.0f;
	SceneProp.ShadowMapResolution = 2048.0f;
	SceneProp.GoodRaysResolution = 0.0f;
	SceneProp.PCFScale = 1.5f;
	SceneProp.PCFSamples = 3.0f;
	SceneProp.ParallaxLowSamples = 20.0f;
	SceneProp.ParallaxHighSamples = 30.0f;
	SceneProp.ParallaxHeight = 0.02f;
	SceneProp.LightVolumeSteps = 256.0f;
	SceneProp.SSAOKernel.Radius = 1.5f;
	SceneProp.SSAOKernel.KernelSize = 32;
#elif defined(MEDIUM_QUALITY)
	SceneProp.DOF_Near_Samples_squared = 1.0f;
	SceneProp.DOF_Far_Samples_squared = 3.0f;
	SceneProp.ShadowMapResolution = 2048.0f;
	SceneProp.GoodRaysResolution = 0.0f;
	SceneProp.PCFScale = 2.1f;
	SceneProp.PCFSamples = 3.0f;
	SceneProp.ParallaxLowSamples = 10.0f;
	SceneProp.ParallaxHighSamples = 18.0f;
	SceneProp.ParallaxHeight = 0.02f;
	SceneProp.LightVolumeSteps = 248.0f;
	SceneProp.SSAOKernel.Radius = 1.5f;
	SceneProp.SSAOKernel.KernelSize = 32;
#elif defined(LOW_QUALITY)
	SceneProp.DOF_Near_Samples_squared = 1.0f;
	SceneProp.DOF_Far_Samples_squared = 2.0f;
	SceneProp.ShadowMapResolution = 1024.0f;
	SceneProp.GoodRaysResolution = 512.0f;
	SceneProp.PCFScale = 1.7f;
	SceneProp.PCFSamples = 1.0f;
	SceneProp.ParallaxLowSamples = 2.0f;
	SceneProp.ParallaxHighSamples = 8.0f;
	SceneProp.ParallaxHeight = 0.01f;
	SceneProp.LightVolumeSteps = 64.0f;
	SceneProp.SSAOKernel.Radius = 1.5f;
	SceneProp.SSAOKernel.KernelSize = 8;
#endif

	SceneProp.SSAOKernel.Update();


	SceneProp.ToogleShadow = true;
	SceneProp.ToogleSSAO = true;
	SceneProp.AutoFocus = true;



	SceneProp.AddGaussKernel(&ShadowFilter);
	SceneProp.AddGaussKernel(&BloomFilter);
	SceneProp.AddGaussKernel(&NearDOFFilter);
	SceneProp.ActiveGaussKernel = SHADOW_KERNEL;
	ChangeActiveGaussSelection = SHADOW_KERNEL;

	RTIndex = -1;

	m_spline.m_points.push_back(t1000::SplinePoint(80, 90, -80));
	m_spline.m_points.back().m_velocity = 3.0f;
	m_spline.m_points.push_back(t1000::SplinePoint(80, 90, -20));
	m_spline.m_points.back().m_velocity = 6.f;
	m_spline.m_points.push_back(t1000::SplinePoint(20, 110, 0));
	m_spline.m_points.back().m_velocity = 20.f;
	m_spline.m_points.push_back(t1000::SplinePoint(30, 20, 0));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(40, 6, 0));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(50, 4, 20));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(40, 4, 20));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(0, 4, 20));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(-50, 3, 20));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(-50, 6, 0));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(-20, 15, 0));
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(0, 10, 0)); //
	m_spline.m_points.back().m_velocity = 7;
	m_spline.m_points.push_back(t1000::SplinePoint(-40, 25, 3));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(-55, 30, 5));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(-55, 30, -25));
	m_spline.m_points.back().m_velocity = 3;
	m_spline.m_points.push_back(t1000::SplinePoint(-65, 25, 0));
	m_spline.m_points.back().m_velocity = 3;
	m_spline.m_points.push_back(t1000::SplinePoint(-60, 25, 25));
	m_spline.m_points.back().m_velocity = 3;
	m_spline.m_points.push_back(t1000::SplinePoint(-35, 30, -20));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(15, 30, -20));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(25, 30, 20));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(50, 30, 20));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(50, 30, -20));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(-50, 30, -20));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(-50, 30, 0));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(-30, 30, 0));
	m_spline.m_points.back().m_velocity = 10;
	m_spline.m_points.push_back(t1000::SplinePoint(0, 5, 0));
	m_spline.m_points.back().m_velocity = 10;

	m_spline.m_looped = false;
	m_spline.Init();

	m_agent.SetOffset(0);
	m_agent.m_pSpline = &m_spline;
	m_agent.m_moving = true;
	m_agent.m_velocity = 15.0f;
}

void TestApp::LoadAssets() {

}

void TestApp::CreateAssets() {
	LogPrintDebug("TestApp::CreateAssets [%d] x [%d]\n", pFramework->pVideoDriver->width, pFramework->pVideoDriver->height);
	DtTimer.Init();
	m_textRender.LoadFromFile(36, "tahomabd.ttf", 512.0f);
	//Create RT's
	int WidthReal = 1280;
	int HeightReal = 720;
	GBufferPass = pFramework->pVideoDriver->CreateRT(4, t1000::BaseRT::RGBA8, t1000::BaseRT::F32,  WidthReal, HeightReal, true);
	DeferredPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::RGBA16F, t1000::BaseRT::NOTHING,  WidthReal, HeightReal, true);
	Extra16FPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::RGBA16F, t1000::BaseRT::NOTHING,  WidthReal, HeightReal, true);
	DepthPass = pFramework->pVideoDriver->CreateRT(0, t1000::BaseRT::NOTHING, t1000::BaseRT::F32, (int)SceneProp.ShadowMapResolution, (int)SceneProp.ShadowMapResolution, false);
	ShadowAccumPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::RGBA8, t1000::BaseRT::NOTHING,  WidthReal, HeightReal, true);
	ExtraHelperPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::RGBA8, t1000::BaseRT::NOTHING,  WidthReal, HeightReal, true);
	BloomAccumPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::RGBA8, t1000::BaseRT::NOTHING, 512, 512, true);
	GodRaysCalcPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::RGBA8, t1000::BaseRT::NOTHING, SceneProp.GoodRaysResolution, SceneProp.GoodRaysResolution, true);
	GodRaysCalcExtraPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::RGBA8, t1000::BaseRT::NOTHING, SceneProp.GoodRaysResolution, SceneProp.GoodRaysResolution, true);
	CoCPass = pFramework->pVideoDriver->CreateRT(2, t1000::BaseRT::F16, t1000::BaseRT::NOTHING, 512, 512, true);
	CombineCoCPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::F16, t1000::BaseRT::NOTHING, 512, 512, true);
	CoCHelperPass = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::F16, t1000::BaseRT::NOTHING, 512, 512, false);
	CoCHelperPass2 = pFramework->pVideoDriver->CreateRT(1, t1000::BaseRT::F16, t1000::BaseRT::NOTHING, 512, 512, false);
	SceneDepthPass = pFramework->pVideoDriver->CreateRT(0, t1000::BaseRT::NOTHING, t1000::BaseRT::F32,  WidthReal, HeightReal, false);

	PrimitiveMgr.Init();
	PrimitiveMgr.SetVP(&VP);
	m_flare.Init(PrimitiveMgr);

	SceneProp.SSAOKernel.InitTexture();

	EnvMapTexIndex = pFramework->pVideoDriver->CreateTexture(string("CubeMap_Mountains.dds"));

	int index = PrimitiveMgr.CreateMesh("SkyBox.X");

	Meshes[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	Meshes[1].TranslateAbsolute(0.0, -10.0f, 0.0f);
	Meshes[1].Update();

	index = PrimitiveMgr.CreateMesh("SponzaEsc.X");
	Meshes[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateSpline(m_spline);

	splineWire = (t1000::SplineWireframe*)PrimitiveMgr.GetPrimitive(index);
	splineInst.CreateInstance(splineWire, &VP);


	m.Identity();

	Quads[0].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);

	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[1], 1);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[2], 2);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[3], 3);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->pDepthTexture, 4);

	Quads[0].SetEnvironmentMap(pFramework->pVideoDriver->GetTexture(EnvMapTexIndex));

	Quads[1].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);
	Quads[2].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);
	Quads[3].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);
	Quads[4].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);
	Quads[5].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);
	Quads[6].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);
	Quads[7].CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &m);

	PrimitiveMgr.SetSceneProps(&SceneProp);

	m_agent.m_actualPoint = m_spline.GetPoint(m_spline.GetNormalizedOffset(0));
	ActiveCam->AttachAgent(m_agent);
	ActiveCam->m_lookAtCenter = false;

	Quads[0].TranslateAbsolute(0.0f, 0.0f, 0.0f);
	Quads[0].Update();

	Quads[1].ScaleAbsolute(0.25);
	Quads[1].TranslateAbsolute(-0.75f, +0.75f, 0.0f);
	Quads[1].Update();

	Quads[2].ScaleAbsolute(0.25f);
	Quads[2].TranslateAbsolute(0.75f, +0.75f, 0.0f);
	Quads[2].Update();

	Quads[3].ScaleAbsolute(0.25f);
	Quads[3].TranslateAbsolute(-0.75f, -0.75f, 0.0f);
	Quads[3].Update();

	Quads[4].ScaleAbsolute(0.25f);
	Quads[4].TranslateAbsolute(0.75f, -0.75f, 0.0f);
	Quads[4].Update();

	Quads[5].ScaleAbsolute(0.25f);
	Quads[5].TranslateAbsolute(0.75f, 0.0f, 0.0f);
	Quads[5].Update();

	Quads[6].ScaleAbsolute(0.25f);
	Quads[6].TranslateAbsolute(-0.75f, 0.0f, 0.0f);
	Quads[6].Update();

	Quads[7].ScaleAbsolute(1.0f);
	Quads[7].TranslateAbsolute(0.0f, 0.0f, 0.1f);
	Quads[7].Update();

 	bInited = true;
}

void TestApp::DestroyAssets() {
	LogPrintDebug("TestApp::DestroyAssets");
	PrimitiveMgr.DestroyPrimitives();
	pFramework->pVideoDriver->DestroyRTs();
}

void TestApp::OnUpdate() {

	OnInput();

	DtTimer.Update();
	DtSecs = DtTimer.GetDTSecs();

	static float timeAccum = 0;
	timeAccum += DtSecs;

	if (timeAccum > 1.0) {
		m_fpsString = "FPS " + patch::to_string((int)(1.0 / DtSecs));
		m_fpsCol = XVECTOR3(0.2, 0.8, 0.2);
		timeAccum = 0;
	}


	Meshes[0].SetParallaxSettings(SceneProp.ParallaxLowSamples, SceneProp.ParallaxHighSamples, SceneProp.ParallaxHeight);
	m_agent.Update(DtSecs);


	ActiveCam->Update(DtSecs);
	VP = ActiveCam->VP;
	SceneProp.Lights[0].Position = LightCam.Eye;
	SceneProp.pLightCameras[0]->Yaw -= 0.008f *DtSecs;
	SceneProp.pLightCameras[0]->Update(DtSecs);
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	

	pFramework->pVideoDriver->SetDepthStencilState(t1000::BaseDriver::DEPTH_STENCIL_STATES::READ_WRITE);

	// Shadow Map Depth Pass
	pFramework->pVideoDriver->PushRT(DepthPass);
	SceneProp.pCameras[0] = &LightCam;
	pFramework->pVideoDriver->SetCullFace(t1000::BaseDriver::FACE_CULLING::BACK_FACES);
	for (int i = 0; i < 2; i++) {
		Meshes[i].SetGlobalSignature(t1000::T_Signature::SHADOW_MAP_PASS);
		Meshes[i].Draw();
		Meshes[i].SetGlobalSignature(t1000::T_Signature::FORWARD_PASS);
	}
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->SetCullFace(t1000::BaseDriver::FACE_CULLING::FRONT_FACES);

	// Only Depth Pass
	pFramework->pVideoDriver->PushRT(SceneDepthPass);
	SceneProp.pCameras[0] = &Cam;
	for (int i = 0; i < 2; i++) {
		Meshes[i].SetGlobalSignature(t1000::T_Signature::SHADOW_MAP_PASS);
		Meshes[i].Draw();
		Meshes[i].SetGlobalSignature(t1000::T_Signature::FORWARD_PASS);
	}
	pFramework->pVideoDriver->PopRT();

	
	// G Buffer Pass
	pFramework->pVideoDriver->PushRT(GBufferPass);
	SceneProp.pCameras[0] = &Cam;
	for (int i = 0; i < 2; i++) {
		Meshes[i].SetGlobalSignature(t1000::T_Signature::GBUFF_PASS);
		Meshes[i].Draw();
		Meshes[i].SetGlobalSignature(t1000::T_Signature::FORWARD_PASS);
	}
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->SetDepthStencilState(t1000::BaseDriver::DEPTH_STENCIL_STATES::READ);

	// Shadow Map Buffer Accumulation + Occlusion 
	pFramework->pVideoDriver->PushRT(ShadowAccumPass);
	pFramework->pVideoDriver->Clear();
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(SceneDepthPass, t1000::BaseDriver::DEPTH_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(DepthPass, t1000::BaseDriver::DEPTH_ATTACHMENT), 1);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, t1000::BaseDriver::COLOR1_ATTACHMENT), 2);
	Quads[0].SetTexture(SceneProp.SSAOKernel.NoiseTex, 3);
	Quads[0].SetGlobalSignature(t1000::T_Signature::SHADOW_COMP_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	// Shadow Map Blur Pass
	pFramework->pVideoDriver->PushRT(ExtraHelperPass);
	SceneProp.ActiveGaussKernel = SHADOW_KERNEL;
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetGlobalSignature(t1000::T_Signature::VERTICAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(ShadowAccumPass);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetGlobalSignature(t1000::T_Signature::HORIZONTAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	// Deferred Pass
	pFramework->pVideoDriver->PushRT(DeferredPass);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, t1000::BaseDriver::COLOR1_ATTACHMENT), 1);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, t1000::BaseDriver::COLOR2_ATTACHMENT), 2);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, t1000::BaseDriver::COLOR3_ATTACHMENT), 3);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(SceneDepthPass, t1000::BaseDriver::DEPTH_ATTACHMENT), 4);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 5);
	Quads[0].SetEnvironmentMap(pFramework->pVideoDriver->GetTexture(EnvMapTexIndex));
	Quads[0].SetGlobalSignature(t1000::T_Signature::DEFERRED_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();


	// God Rays and Volumetric Pass
	pFramework->pVideoDriver->PushRT(GodRaysCalcPass);
	Quads[0].SetGlobalSignature(t1000::T_Signature::LIGHT_RAY_MARCHING);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(SceneDepthPass, t1000::BaseDriver::DEPTH_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(DepthPass, t1000::BaseDriver::DEPTH_ATTACHMENT), 1);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	//God Rays blur
	pFramework->pVideoDriver->PushRT(GodRaysCalcExtraPass);
	SceneProp.ActiveGaussKernel = DOF_KERNEL;
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GodRaysCalcPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetGlobalSignature(t1000::T_Signature::VERTICAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(GodRaysCalcPass);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GodRaysCalcExtraPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetGlobalSignature(t1000::T_Signature::HORIZONTAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(Extra16FPass);
	Quads[0].SetGlobalSignature(t1000::T_Signature::LIGHT_ADD);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GodRaysCalcPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 1);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();



	// Bright Pass
	pFramework->pVideoDriver->PushRT(BloomAccumPass);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(Extra16FPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetGlobalSignature(t1000::T_Signature::BRIGHT_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	SceneProp.ActiveGaussKernel = BLOOM_KERNEL;
	pFramework->pVideoDriver->PushRT(ExtraHelperPass);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetGlobalSignature(t1000::T_Signature::HORIZONTAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(BloomAccumPass);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetGlobalSignature(t1000::T_Signature::VERTICAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();


	SceneProp.ActiveGaussKernel = DOF_KERNEL;
	//DOF PASS
	pFramework->pVideoDriver->PushRT(CoCPass);
	Quads[0].SetGlobalSignature(t1000::T_Signature::COC_PASS);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(SceneDepthPass, t1000::BaseDriver::DEPTH_ATTACHMENT), 0);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	//COMBINE COC
	pFramework->pVideoDriver->PushRT(CombineCoCPass);
	Quads[0].SetGlobalSignature(t1000::T_Signature::COMBINE_COC_PASS);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(CoCPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(CoCPass, t1000::BaseDriver::COLOR1_ATTACHMENT), 1);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();
	////DOF_BLUR
	pFramework->pVideoDriver->PushRT(DeferredPass);
	Quads[0].SetGlobalSignature(t1000::T_Signature::DOF_PASS);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(Extra16FPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(CombineCoCPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 1);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(Extra16FPass);
	Quads[0].SetGlobalSignature(t1000::T_Signature::DOF_PASS_2);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(CombineCoCPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 1);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	// HDR Composition Pass
	pFramework->pVideoDriver->PushRT(ExtraHelperPass);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(Extra16FPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 1);
	Quads[0].SetGlobalSignature(t1000::T_Signature::HDR_COMP_PASS);
	Quads[0].Draw();


	pFramework->pVideoDriver->PopRT();

	// Final Draw
	Quads[7].SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[7].SetGlobalSignature(t1000::T_Signature::VIGNETTE_PASS);
	Quads[7].Draw();
	/*
	Quads[1].SetTexture(pFramework->pVideoDriver->GetRTTexture(DepthPass, t1000::BaseDriver::DEPTH_ATTACHMENT), 0);
	Quads[1].SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	Quads[1].Draw();

	Quads[2].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, t1000::BaseDriver::COLOR1_ATTACHMENT), 0);
	Quads[2].SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	Quads[2].Draw();

	Quads[3].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, t1000::BaseDriver::COLOR2_ATTACHMENT), 0);
	Quads[3].SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	Quads[3].Draw();

	Quads[4].SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, t1000::BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[4].SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	Quads[4].Draw();
	*/
	if (SceneProp.pCameras[0]->Eye.y > 80) {
		m_flare.Draw();
	}

	m_textRender.Draw(-0.9f, -0.8f, m_fpsCol, m_fpsString);

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
