#pragma once

#include <Core/Core.h>
#include <scene/PrimitiveManager.h>
#include <scene/PrimitiveInstance.h>
#include <scene/SceneProp.h>

#include <utils/xMaths.h>
#include <utils/Camera.h>
#include <utils/Timer.h>

struct Sprite;
struct stateBase {
	virtual void Enter(Sprite*) = 0;
	virtual void Run(Sprite*,float) = 0;
	virtual void Exit(Sprite*) = 0;
};

struct Sprite_on_Back_State : public stateBase {
	void Enter(Sprite*);
	void Run(Sprite*, float);
	void Exit(Sprite*);
};

struct Sprite_Rotating_to_Front_State : public stateBase {
	void Enter(Sprite*);
	void Run(Sprite*, float);
	void Exit(Sprite*);
};

struct Sprite_on_Front_State : public stateBase {
	void Enter(Sprite*);
	void Run(Sprite*, float);
	void Exit(Sprite*);
};

struct Sprite_Rotating_to_Back_State : public stateBase {
	void Enter(Sprite*);
	void Run(Sprite*, float);
	void Exit(Sprite*);
};

struct Pair {
	Pair() : ImageTexture(0), TextTexture(0), DescriptionTexture(0), Id(0), Taken(0) {}
	t1000::Texture *ImageTexture;
	t1000::Texture *TextTexture;
	t1000::Texture *DescriptionTexture;
	int Taken;
	int	Id;
};

struct Sprite {
	Sprite() :
		TexBG(0),
		ActualTexture(0),
		DescriptionTexture(0),
		Equal(0),
		CurrentState(0),
		Angle(0.0f),
		PosXOriginal(0.0f),
		PosYOriginal(0.0f),
		ScaleXOriginal(1.0f),
		ScaleYOriginal(1.0f),
		State(BACK),
		Taken(0),
		Type(0),
		NeedtoChangeState(0),
		Collision(0),
		SpritesOnFront(0),
		SpritesOnBack(0),
		SpritesDisabled(0),
		Enabled(0),
		Sign(1.0f),
		SpeedOnDegrees(270.0f)
	{}

	enum STATE {
		BACK, SPIN_FRONT, SPIN_BACK, FRONT
	};
	
	enum TYPE {
		IMAGE, TEXT
	};

	void Create(t1000::PrimitiveBase* base,float x, float y, float scx, float scy);

	void SetTexture(t1000::Texture* tex);

	void ChangeState(stateBase *state);

	void Update(float dt);

	void CheckForCollision(float coordx, float coordY);

	void Rotate180();

	void Draw();

	t1000::Texture			*TexBG;
	t1000::Texture			*ActualTexture;
	t1000::Texture			*DescriptionTexture;
	t1000::PrimitiveInst	pInstance;
	Sprite *Equal;
	stateBase *CurrentState;
	std::string Name;
	float	Angle;
	float	PosXOriginal, PosYOriginal;
	float	ScaleXOriginal,ScaleYOriginal;
	int		State;
	int		Taken;
	int		Type;
	int		NeedtoChangeState;
	int		Collision;
	int		SpritesOnFront;
	int		SpritesOnBack;
	int		SpritesDisabled;
	int		Enabled;
	float	Sign;
	float	SpeedOnDegrees;
};

class TestApp;

struct stateGameBase {
	virtual void Enter(TestApp*) = 0;
	virtual void Update(TestApp*, float) = 0;
	virtual void Draw(TestApp*, float) = 0;
	virtual void Exit(TestApp*) = 0;
};

struct StartWindow : public stateGameBase {
	void Enter(TestApp*);
	void Update(TestApp*, float);
	void Draw(TestApp*, float);
	void Exit(TestApp*);
};

struct PlayingtWindow : public stateGameBase {
	void Enter(TestApp*);
	void Update(TestApp*, float);
	void Draw(TestApp*, float);
	void Exit(TestApp*);
};

struct PlayingtWindowTransitionToDesc : public stateGameBase {
	void Enter(TestApp*);
	void Update(TestApp*, float);
	void Draw(TestApp*, float);
	void Exit(TestApp*);
};

struct PlayingtWindowDescription: public stateGameBase {
	void Enter(TestApp*);
	void Update(TestApp*, float);
	void Draw(TestApp*, float);
	void Exit(TestApp*);
};

struct PlayingtWindowTransitionToPlay : public stateGameBase {
	void Enter(TestApp*);
	void Update(TestApp*, float);
	void Draw(TestApp*, float);
	void Exit(TestApp*);
};

struct PlayingtWindowToEnd : public stateGameBase {
	void Enter(TestApp*);
	void Update(TestApp*, float);
	void Draw(TestApp*, float);
	void Exit(TestApp*);
};



class TestApp : public t1000::AppBase {
public:
	TestApp() : t1000::AppBase() {}

	void InitVars();

	void LoadAssets();

	void ChangeState(stateGameBase*t);

	void CreateAssets();
	void DestroyAssets();

	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	void LoadScene(int id);

	void Randomize();

	t1000::PrimitiveManager PrimitiveMgr;
//	t1000::PrimitiveInst	Sprites[16];
	t1000::PrimitiveInst    Meshes[16];
	t1000::PrimitiveInst	BGInstance;
	t1000::PrimitiveInst	SplashInstance;
	t1000::PrimitiveInst	WinningInstance;

	stateGameBase		*pCurrentState;

	t1000::Texture			*TexBG;
	t1000::Texture			*Splash;
	t1000::Texture			*GeneralTexBG;
	t1000::Texture			*DescriptionToShow;

	Pair			Pairs[8];

	Sprite			Sprites[16];

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

	int				TexIdTest;
	int				SpritesDisabled;

	Timer			DtTimer;
	float			DtSecs;
	float			grdo;

	float			realXCoord;
	float			realYCoord;

	float			WinningScale;
};