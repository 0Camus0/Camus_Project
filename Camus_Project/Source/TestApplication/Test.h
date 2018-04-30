#pragma once

#include <Core/Core.h>
#include <scene/PrimitiveManager.h>
#include <scene/PrimitiveInstance.h>
#include <scene/SceneProp.h>
#include <scene/T8_TextRenderer.h>

#include <utils/xMaths.h>
#include <utils/Camera.h>
#include <utils/Timer.h>

class TestApp : public t1000::AppBase {
public:
	TestApp() : t1000::AppBase() {}

	void InitVars();

	void LoadAssets();

	

	void CreateAssets();
	void DestroyAssets();

	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	void LoadScene(int id);

	t1000::PrimitiveManager PrimitiveMgr;
	t1000::PrimitiveInst	Sprite;
	t1000::PrimitiveInst    Meshes[10];

	Camera			Cam;
	Camera			LightCam;

	GaussFilter		ShadowFilter;
	GaussFilter		BloomFilter;
	GaussFilter		NearDOFFilter;

	Camera			*ActiveCam;

	XVECTOR3		Position;
	XVECTOR3		Orientation;
	XVECTOR3		Scaling;

	XMATRIX44		View;
	XMATRIX44		Projection;
	XMATRIX44		VP;

	SceneProps		SceneProp;

	Timer			DtTimer;
	float			DtSecs;
};