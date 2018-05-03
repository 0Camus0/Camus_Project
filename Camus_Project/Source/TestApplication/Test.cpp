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

#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>

Sprite_on_Back_State			*onBackState = 0;
Sprite_Rotating_to_Front_State	*onRotatingToFrontState = 0;;
Sprite_on_Front_State			*onFrontState = 0;;
Sprite_Rotating_to_Back_State	*onRotatingToBackState = 0;;


StartWindow						*onStartWindow;
PlayingtWindow					*onPlayingWindow;
PlayingtWindowTransitionToDesc	*onPlayingtWindowTransitionToDesc;
PlayingtWindowDescription		*onPlayingtWindowDescription;
PlayingtWindowTransitionToPlay  *onPlayingtWindowTransitionToPlay;
PlayingtWindowToEnd				*onPlayingtWindowToEnd;


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

	onBackState = new Sprite_on_Back_State;
	onRotatingToFrontState = new Sprite_Rotating_to_Front_State;
	onFrontState = new Sprite_on_Front_State;
	onRotatingToBackState = new Sprite_Rotating_to_Back_State;


	onStartWindow = new StartWindow;
	onPlayingWindow = new PlayingtWindow;
	onPlayingtWindowTransitionToDesc = new PlayingtWindowTransitionToDesc;
	onPlayingtWindowDescription = new PlayingtWindowDescription;
	onPlayingtWindowTransitionToPlay = new PlayingtWindowTransitionToPlay;
	onPlayingtWindowToEnd = new PlayingtWindowToEnd;

	pCurrentState = onStartWindow;
}

void TestApp::LoadAssets() {

}

void TestApp::ChangeState(stateGameBase *st) {
	st->Exit(this);
	pCurrentState = st;
	st->Enter(this);
}

void TestApp::CreateAssets() {
	LogPrintDebug("TestApp::CreateAssets [%d] x [%d]\n", pFramework->pVideoDriver->width, pFramework->pVideoDriver->height);
	DtTimer.Init();

	struct timeval time;
	gettimeofday(&time, NULL);
	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	PrimitiveMgr.Init();
	PrimitiveMgr.SetVP(&VP);


	int index = 0; //= PrimitiveMgr.CreateMesh("Scene.X");
	
	std::string Texts[] = { "Disenio.tga" , "DistribuciondeMedicamentos.tga", "Dodis.tga", "Evolucion.tga", "FarmaciaHospitalaria.tga", "Gestion.tga", "Monitorizacion.tga", "Prescripcion.tga" };
	std::string Images[] = { "DisenioIMG.tga" , "DistribuciondeMedicamentosIMG.tga", "DodisIMG.tga", "EvolucionIMG.tga", "FarmaciaHospitalariaIMG.tga", "GestionIMG.tga", "MonitorizacionIMG.tga", "PrescripcionIMG.tga" };
	std::string Desc[] = { "DisenioTEXT.tga" , "DistribuciondeMedicamentosTEXT.tga", "DodisTEXT.tga", "EvolucionTEXT.tga", "FarmaciaHospitalariaTEXT.tga", "GestionTEXT.tga", "MonitorizacionTEXT.tga", "PrescripcionTEXT.tga" };

	for (std::size_t i = 0; i < 8; i++) {
		TexIdTest = pFramework->pVideoDriver->CreateTexture(Images[i]);
		Pairs[i].ImageTexture = pFramework->pVideoDriver->GetTexture(TexIdTest);
		TexIdTest = pFramework->pVideoDriver->CreateTexture(Texts[i]);
		Pairs[i].TextTexture = pFramework->pVideoDriver->GetTexture(TexIdTest);
		TexIdTest = pFramework->pVideoDriver->CreateTexture(Desc[i]);
		Pairs[i].DescriptionTexture = pFramework->pVideoDriver->GetTexture(TexIdTest);
		Pairs[i].Id = i;
		Pairs[i].Taken = 0;
	}

	TexBG = pFramework->pVideoDriver->GetTexture(pFramework->pVideoDriver->CreateTexture("BG.tga"));
	Splash = pFramework->pVideoDriver->GetTexture(pFramework->pVideoDriver->CreateTexture("Splash.tga"));
	GeneralTexBG = pFramework->pVideoDriver->GetTexture(pFramework->pVideoDriver->CreateTexture("GeneralBG.tga"));

	float win_w = (float)pFramework->pVideoDriver->width;
	float win_h = (float)pFramework->pVideoDriver->height;
	float w = 1024.0f; // (float)Textures[0]->x;
	float h = 768.0f; // (float)Textures[0]->y;

	float width_ = (float)win_w;
	float height_ = (float)win_h;

	float image_w = w;
	float image_h = h;

	float ratio_y = (image_h / height_);
	float ratio_x = image_w / width_ / ratio_y;

	float general_Scale = 0.2f;

	float OriginX = -1.0f + 2.0f*0.1;
	float OriginY = 1.0f - 2.0f*0.111;

	float startX = OriginX;
	float startY = OriginY;
	int counter = 0;
	float FXScale = 1.0f;
	float FYScale = 1.0f;

	for (int i = 0; i < 16; i++) {

		if (ratio_x < 1.0f) {
			FXScale = ratio_x * general_Scale;
			FYScale = 1.0f*general_Scale;;		}
		else {
			ratio_x = image_w / width_;
			ratio_y = (image_h / height_) / ratio_x;
			FXScale = 1.0f*general_Scale;
			FYScale = ratio_y * general_Scale;
		}

		Sprites[i].Create(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), startX, startY, FXScale, FYScale);
		Sprites[i].TexBG = TexBG;
		Sprites[i].CurrentState = onBackState;

		counter++;

		if (counter == 4) {
			startX = OriginX;
			startY -= 1.0f / 2.0f;
			counter = 0;
		}
		else {
			startX += 1.0f / 2.0f;
		}
	}

	BGInstance.CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &VP);
	BGInstance.SetTexture(GeneralTexBG, 0);

	SplashInstance.CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &VP);
	SplashInstance.SetTexture(Splash,0);

	WinningInstance.CreateInstance(PrimitiveMgr.GetPrimitive(t1000::PrimitiveManager::QUAD), &VP);
	
	Randomize();
	

	PrimitiveMgr.SetSceneProps(&SceneProp);
	
	grdo = 0.0f;
 	bInited = true;
}

void TestApp::Randomize() {
	for (std::size_t i = 0; i < 8; i++) {
		int random_1 = RandomRange(0, 15);
		int random_2 = RandomRange(0, 15);

		while (random_1 == random_2 || Sprites[random_1].Taken == 1 || Sprites[random_2].Taken == 1) {
			random_1 = RandomRange(0, 15);
			random_2 = RandomRange(0, 15);		
		}

		Sprites[random_1].ActualTexture = Pairs[i].ImageTexture;
		Sprites[random_1].Type = Sprite::IMAGE;
		Sprites[random_2].ActualTexture = Pairs[i].TextTexture;
		Sprites[random_2].Type = Sprite::TEXT;
		Sprites[random_1].DescriptionTexture = Pairs[i].DescriptionTexture;
		Sprites[random_2].DescriptionTexture = Pairs[i].DescriptionTexture;
		Sprites[random_1].Equal = &Sprites[random_2];
		Sprites[random_2].Equal = &Sprites[random_1];
		Sprites[random_1].Taken = 1;
		Sprites[random_2].Taken = 1;
		Sprites[random_1].Enabled = 1;
		Sprites[random_2].Enabled = 1;
//		LogPrintDebug("Sprite %d assign %d Tex[%d] name[%s] TexEqual name[%s]", random_1, random_2, i, Sprites[random_1].ActualTexture->optname, Sprites[random_1].Equal->ActualTexture->optname);
	}

	for (int i = 0; i < 16; i++) {
		Sprites[i].SetTexture(Sprites[i].ActualTexture);
		Sprites[i].Taken = 0;
		Sprites[i].ChangeState(onBackState);
	}
}

void TestApp::DestroyAssets() {
	LogPrintDebug("TestApp::DestroyAssets");
}

void TestApp::OnUpdate() {

	OnInput();

	DtTimer.Update();
	DtSecs = DtTimer.GetDTSecs();


	
	pCurrentState->Update(this, DtSecs);

	//grdo += DtSecs;

	//if (grdo > 2.0f) {
	//	Randomize();
	//	grdo = 0.0f;
	//}

	ActiveCam->Update(DtSecs);
	VP = ActiveCam->VP;

}

void TestApp::OnDraw() {

	pFramework->pVideoDriver->Clear();

	BGInstance.SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	BGInstance.SetTexture(GeneralTexBG, 0);
	BGInstance.Draw();
	

	pCurrentState->Draw(this, DtSecs);

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

void Sprite::Create(t1000::PrimitiveBase* base, float x, float y, float scx, float scy) {
	XMATRIX44 VP;
	pInstance.CreateInstance(base, &VP);	
	PosXOriginal = x;
	PosYOriginal = y;
	ScaleXOriginal = scx;
	ScaleYOriginal = scy;
}

void Sprite::ChangeState(stateBase *state) {
	CurrentState->Exit(this);
	CurrentState = state;
	CurrentState->Enter(this);
}

void Sprite::SetTexture(t1000::Texture* tex) {
	pInstance.SetTexture(tex,0);
}

void Sprite::Update(float dt) {
	CurrentState->Run(this,dt);
}

void Sprite::CheckForCollision(float coordx, float coordy) {
	float x_delta = coordx - PosXOriginal;
	float y_delta = coordy - PosYOriginal;
	float distance_ = sqrtf((x_delta*x_delta) + (y_delta* y_delta));

	if (distance_ < 0.1f) {
		Collision = true;
		LogPrintDebug("Collision!");
	}
	else {
		Collision = false;
	}
}

void Sprite::Rotate180() {

}

void Sprite::Draw() {
	pInstance.SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	pInstance.Draw();
}

// Back
void Sprite_on_Back_State::Enter(Sprite* sp) {
	sp->Angle = 0.0f;
	sp->Sign = 1.0f;
	sp->SetTexture(sp->TexBG);
	sp->State = Sprite::BACK;
}

void Sprite_on_Back_State::Run(Sprite* sp,float dt) {
	sp->pInstance.TranslateAbsolute(sp->PosXOriginal, sp->PosYOriginal, 0.0f);
	sp->pInstance.ScaleAbsolute(sp->ScaleXOriginal*sp->Sign, sp->ScaleYOriginal, 1.0f);
	sp->pInstance.RotateYAbsolute(sp->Angle);
	sp->pInstance.Update();

	if (sp->Collision && sp->SpritesOnBack > (14 - sp->SpritesDisabled)) {
		sp->NeedtoChangeState = true;
		sp->Collision = false;
	}

	if (sp->NeedtoChangeState)
		sp->ChangeState(onRotatingToFrontState);
}

void Sprite_on_Back_State::Exit(Sprite* sp) {
	sp->NeedtoChangeState = 0;
}

// To Front
void Sprite_Rotating_to_Front_State::Enter(Sprite* sp) {
	sp->Angle = 0.0f;
	sp->SetTexture(sp->TexBG);
	sp->State = Sprite::SPIN_FRONT;
}

void Sprite_Rotating_to_Front_State::Run(Sprite* sp, float dt) {
	sp->Angle += sp->SpeedOnDegrees*dt;


	if (sp->Angle >= 90.0f) {
		sp->SetTexture(sp->ActualTexture);
		sp->Sign = -1.0f;
	}

	if (sp->Angle >= 180.0f) {
		sp->Angle = 180.0f;
		sp->ChangeState(onFrontState);
	}


	sp->pInstance.TranslateAbsolute(sp->PosXOriginal, sp->PosYOriginal, 0.0f);
	sp->pInstance.ScaleAbsolute(sp->ScaleXOriginal*sp->Sign, sp->ScaleYOriginal, 1.0f);
	sp->pInstance.RotateYAbsolute(sp->Angle);
	sp->pInstance.Update();
}

void Sprite_Rotating_to_Front_State::Exit(Sprite* sp) {
	sp->NeedtoChangeState = 0;
}

// Front
void Sprite_on_Front_State::Enter(Sprite* sp) {
	sp->Angle = 180.0f;
	sp->SetTexture(sp->ActualTexture);
	sp->Sign = -1.0f;
	sp->State = Sprite::FRONT;
}

void Sprite_on_Front_State::Run(Sprite* sp, float dt) {
	sp->pInstance.TranslateAbsolute(sp->PosXOriginal, sp->PosYOriginal, 0.0f);
	sp->pInstance.ScaleAbsolute(sp->ScaleXOriginal*sp->Sign, sp->ScaleYOriginal, 1.0f);
	sp->pInstance.RotateYAbsolute(sp->Angle);
	sp->pInstance.Update();

	if (sp->Collision) {
		sp->NeedtoChangeState = true;
		sp->Collision = false;
	}

	if (sp->NeedtoChangeState)
		sp->ChangeState(onRotatingToBackState);
}

void Sprite_on_Front_State::Exit(Sprite* sp) {
	sp->NeedtoChangeState = 0;
}

// To Back
void Sprite_Rotating_to_Back_State::Enter(Sprite* sp) {
	sp->Angle = 180.0f;
	sp->SetTexture(sp->ActualTexture);
	sp->Sign = -1.0f;
	sp->State = Sprite::SPIN_BACK;
}

void Sprite_Rotating_to_Back_State::Run(Sprite* sp, float dt) {
	sp->Angle -= sp->SpeedOnDegrees*dt;


	if (sp->Angle <= 90.0f) {
		sp->SetTexture(sp->TexBG);
		sp->Sign = 1.0f;
	}

	if (sp->Angle <= 0.0f) {
		sp->Angle = 0.0f;
		sp->ChangeState(onBackState);
	}

	sp->pInstance.TranslateAbsolute(sp->PosXOriginal, sp->PosYOriginal, 0.0f);
	sp->pInstance.ScaleAbsolute(sp->ScaleXOriginal*sp->Sign, sp->ScaleYOriginal, 1.0f);
	sp->pInstance.RotateYAbsolute(sp->Angle);
	sp->pInstance.Update();
}

void Sprite_Rotating_to_Back_State::Exit(Sprite* sp) {
	sp->NeedtoChangeState = 0;
}

void StartWindow::Enter(TestApp* app) {
	app->SpritesDisabled = 0;
	app->WinningScale = 1.0f;
	for (int i = 0; i < 16; i++) {
		app->Sprites[i].SpritesDisabled = 0;
	}
	app->Randomize();
}

void StartWindow::Update(TestApp* app, float) {
	int xCoord = 0, yCoord = 0;
	bool bChange = false;

	if (app->IManager.PressedOnceTouch(xCoord, yCoord)) {
		bChange = true;
	}

	if (bChange)
		app->ChangeState(onPlayingWindow);

	app->SplashInstance.ScaleAbsolute(app->WinningScale, app->WinningScale, app->WinningScale);
	 
}

void StartWindow::Draw(TestApp* app, float) {
	app->SplashInstance.SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	app->SplashInstance.SetTexture(app->Splash, 0);
	app->SplashInstance.Draw();
}
void StartWindow::Exit(TestApp* app) {

}



void PlayingtWindow::Enter(TestApp* app ) {

}

void PlayingtWindow::Update(TestApp* app, float DtSecs) {
	int xCoord = 0, yCoord = 0;


	float win_w = (float)app->pFramework->pVideoDriver->width;
	float win_h = (float)app->pFramework->pVideoDriver->height;

	int OnBack = 0, OnFront = 0;
	for (int i = 0; i < 16; i++) {
		if (app->Sprites[i].Enabled) {
			if (app->Sprites[i].State == Sprite::BACK)
				OnBack++;
			if (app->Sprites[i].State == Sprite::FRONT)
				OnFront++;
		}
	}

	if (app->IManager.PressedOnceTouch(xCoord, yCoord)) {
		float realXCoord = (((float)xCoord) / win_w)*2.0 - 1.0;
		float realYCoord = 2.0 - (((float)yCoord) / win_h)*2.0 - 1.0;


		for (int i = 0; i < 16; i++) {
			if (app->Sprites[i].Enabled) {
				app->Sprites[i].SpritesOnFront = OnFront;
				app->Sprites[i].SpritesOnBack = OnBack;
				app->Sprites[i].CheckForCollision(realXCoord, realYCoord);
			}
		}
	}

	if (OnFront == 2) {
		int count = 0;
		Sprite *both[2] = { 0 , 0 };
		for (int i = 0; i < 16; i++) {
			if (app->Sprites[i].Enabled) {
				if (app->Sprites[i].State == Sprite::FRONT) {
					both[count++] = &app->Sprites[i];
				}
			}
		}

		if (both[0]->Equal == both[1]) {
			both[0]->Enabled = 0;
			both[1]->Enabled = 0;
			app->SpritesDisabled += 2;
			for (int i = 0; i < 16; i++) {		
				app->Sprites[i].SpritesDisabled = app->SpritesDisabled;
			}
			app->DescriptionToShow = both[0]->DescriptionTexture;
			app->ChangeState(onPlayingtWindowTransitionToDesc);
		}

	}

	for (int i = 0; i < 16; i++) {
		app->Sprites[i].Update(DtSecs);
	}


}

void PlayingtWindow::Draw(TestApp* app, float DtSecs) {
	for (int i = 0; i < 16; i++) {
		if (app->Sprites[i].Enabled) 
			app->Sprites[i].Draw();
	}
}

void PlayingtWindow::Exit(TestApp* app) {

}



void PlayingtWindowTransitionToDesc::Enter(TestApp* app) {
	app->WinningScale = 0.0f;
	app->WinningInstance.SetTexture(app->DescriptionToShow, 0);
}

void PlayingtWindowTransitionToDesc::Update(TestApp* app, float dt) {
	app->WinningScale += 1.0f*dt;

	if (app->WinningScale >= 1.0f) {
		app->WinningScale = 1.0f;
		app->ChangeState(onPlayingtWindowDescription);
	}

	app->WinningInstance.ScaleAbsolute(app->WinningScale, app->WinningScale, app->WinningScale);
	app->WinningInstance.Update();
}

void PlayingtWindowTransitionToDesc::Draw(TestApp* app, float) {
	app->WinningInstance.SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	app->WinningInstance.SetTexture(app->DescriptionToShow, 0);
	app->WinningInstance.Draw();
}

void PlayingtWindowTransitionToDesc::Exit(TestApp* app) {
	app->WinningScale = 1.0f;
}



void PlayingtWindowDescription::Enter(TestApp* app) {

}

void PlayingtWindowDescription::Update(TestApp* app, float) {
	int xCoord = 0, yCoord = 0;
	bool bChange = false;

	if (app->IManager.PressedOnceTouch(xCoord, yCoord)) {
		bChange = true;
	}

	int counterF = 0;
	for (int i = 0; i < 16; i++) {
		if (!app->Sprites[i].Enabled)
			counterF++;
	}

	if (counterF == 16 && bChange) {
		app->ChangeState(onStartWindow);
		return;
	}

	if (bChange)
		app->ChangeState(onPlayingWindow);

	app->WinningInstance.ScaleAbsolute(app->WinningScale, app->WinningScale, app->WinningScale);

	
}

void PlayingtWindowDescription::Draw(TestApp* app, float) {
	app->WinningInstance.SetGlobalSignature(t1000::T_Signature::FSQUAD_TESTING);
	app->WinningInstance.SetTexture(app->DescriptionToShow, 0);
	app->WinningInstance.Draw();
		
}

void PlayingtWindowDescription::Exit(TestApp* app) {

}



void PlayingtWindowTransitionToPlay::Enter(TestApp*) {

}

void PlayingtWindowTransitionToPlay::Update(TestApp* app, float) {

}

void PlayingtWindowTransitionToPlay::Draw(TestApp* app, float) {

}

void PlayingtWindowTransitionToPlay::Exit(TestApp*) {

}



void PlayingtWindowToEnd::Enter(TestApp*) {

}

void PlayingtWindowToEnd::Update(TestApp* app, float) {

}

void PlayingtWindowToEnd::Draw(TestApp* app, float) {

}

void PlayingtWindowToEnd::Exit(TestApp*) {

}

