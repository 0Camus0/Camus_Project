#include "Test.h"
#include <Utils/Log.h>
#include <Driver/Technique.h>
#include <Driver/EffectGL.h>
#include <Driver/TextureGL.h>

#ifdef OS_WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

#include <algorithm>
#include <iostream>
#include <cmath>


float clip(float n, float lower, float upper) {
	return std::max(
		lower,
		std::min(n, upper));
}
 
void TestApp::CreateAssets() {
	LogPrintDebug("TestApp::CreateAssets\n");

	hyperspace::video::CTechnique_ *AllTechnique = new hyperspace::video::TechniqueGL();
	AllTechnique->Initialize("All-tech", "All");

//	delete AllTechnique;


	hyperspace::video::TextureManager	*TexManager = new hyperspace::video::TextureManagerGL();

	/*
	TexManager->LoadTexture("pvr/BatBody_pvrtc2_rgba.pvr");
	TexManager->LoadTexture("pvr/BatHead_pvrtc2_rgb.pvr"); 
	TexManager->LoadTexture("pvr/CrocBody_pvrtc4_rgba.pvr");
	TexManager->LoadTexture("pvr/CrocHead_pvrtc4_rgb.pvr");*
	TexManager->LoadTexture("pvr/JokerBody_pvrtcII_2bpp.pvr");
	TexManager->LoadTexture("pvr/JokerHead_pvrtcII_4bpp.pvr");*/
	

	/*TexManager->LoadTexture("ktx/textetc1.ktx");
	TexManager->LoadTexture("ktx/cube.ktx");

	TexManager->LoadTexture("pvr/Cubemap.pvr");*/
	//TexManager->LoadTexture("ktx/EVEN_MONEY_ETC1.ktx");
	TexManager->LoadTexture("pvr/EVEN_MONEY_ETC1.pvr");
	/*TexManager->LoadTexture("pvr/cabeza_dxt1.pvr");
	TexManager->LoadTexture("pvr/traje_dxt5.pvr");*/


	/*
	TexManager->LoadTexture("cube_xn.png");
	TexManager->LoadTexture("Real.png");
	TexManager->LoadTexture("bones.bmp");
	TexManager->LoadTexture("floor_normal.tga");
	*/ 

//	delete TexManager;


	
	bInited = true;
}

void TestApp::DestroyAssets() {
	LogPrintDebug("TestApp::DestroyAssets");
}

void TestApp::OnUpdate(unsigned int dt) {

}

void TestApp::OnDraw() {
	static float ang = 0.0f;

	float R = 0.0f, G = 0.0f, B = 0.0f;

	ang += 0.0001f;

	R = (clip(std::sin(ang), 0.0f, 1.0f))*0.5f + 0.5f;
	G = (clip(std::cos(ang + .70f), 0.0f, 1.0f))*0.5f + 0.5f;
	B = (clip(std::tan(ang + 1.44f), 0.0f, 1.0f))*0.5f + 0.5f;

	pFramework->pVideoDriver->Clear(hyperspace::video::draw_bits_::COLOR_BIT, R, G, B, 1.0f);

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
}