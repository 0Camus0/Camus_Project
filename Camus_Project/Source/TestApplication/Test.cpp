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
	TexManager->LoadTexture("etc1/BatBody_ETC1.ktx");
	TexManager->LoadTexture("etc1/BatHead_ETC1.ktx");
	TexManager->LoadTexture("etc1/CrocBody_ETC1.ktx");
	TexManager->LoadTexture("etc1/CrocHead_ETC1.ktx");
	TexManager->LoadTexture("etc1/cube_ETC1.ktx");
	TexManager->LoadTexture("etc1/JokerBody_ETC1.ktx");
	TexManager->LoadTexture("etc1/JokerHead_ETC1.ktx");
	TexManager->LoadTexture("etc1/text_ETC1.ktx");
	*/
/*
	TexManager->LoadTexture("bones.bmp");
	TexManager->LoadTexture("etc1/BatHead_ETC1.ktx");
	TexManager->LoadTexture("etc1/CrocBody_ETC1.ktx");
	TexManager->LoadTexture("etc1/CrocHead_ETC1.ktx");
	TexManager->LoadTexture("etc1/cube_ETC1.ktx");
	TexManager->LoadTexture("Real.png");
	TexManager->LoadTexture("etc1/JokerHead_ETC1.ktx");
	TexManager->LoadTexture("floor_normal.tga");
	*/
	
	
	TexManager->AddTextureToLoadingQueue("etc1/BatBody_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/BatHead_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/CrocBody_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/CrocHead_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/cube_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/JokerBody_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/JokerHead_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/text_ETC1.ktx");
	
	
	TexManager->AddTextureToLoadingQueue("bones.bmp");
	TexManager->AddTextureToLoadingQueue("etc1/BatHead_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/CrocBody_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/CrocHead_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("etc1/cube_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("Real.png");
	TexManager->AddTextureToLoadingQueue("etc1/JokerHead_ETC1.ktx");
	TexManager->AddTextureToLoadingQueue("floor_normal.tga");
	
	TexManager->LoadTextureQueue();
	
	
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