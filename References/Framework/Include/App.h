/*=============================================================================
GApp.h : Default Application Class.
Source Control:
Created by Camus
=============================================================================*/

#ifndef GAPP_H
#define GAPP_H



#include "Framework.h"
#include "OpenglDrv.h"
#include "Timer.h"
#include "FTMaths.h"
#include "Cam.h"
#include "TextureContainer.h"

#include "../../XLoader/Include/XFileParser.h"

#include "../../XLoader/Include/XDataBase.h"

#include "Plane.h"
#include "StaticMesh.h"
#include "Effect.h"
#include <FTGL/ftgl.h>

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class GApp : public Base
{
public:
	GApp();
	~GApp(){ Release(); }
	virtual void Init();
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float globalTime);
	virtual void OnInput(float globalTime);
	virtual void OnRender();
	virtual void Release();

	inline void	SetFrameWork(OGLFramework	*pFramework){ pFrame = pFramework; }

private:

	void	CreateFBORelfect();
	void	CreateFBOShadowMap();


	void	DrawMeshes();

	void	DrawEffectCall(STD_Static_Mesh *Mesh,STDMATRIX &Mat,STDMATRIX &MatShadow);
	void	DrawEffectCallToDepth(STD_Static_Mesh *Mesh,STDMATRIX &Mat);
	void	DrawReflect();
	void	DrawShadowMap();

	OGLFramework	*pFrame;
	bool			FirstFrame;

	FTFont			*Font;

	STDMATRIX		 World;
	STDMATRIX		 MatrizMonosInvertida;
	STDMATRIX	     MatrizMonosNormal;
	STDMATRIX		 MatrizPiso;

	STDMATRIX		WorldNewMeshes;

	Cam				 Camera;
	Cam				 LightCamera;

	STDVECTOR3		LightPos;

	Plane			Planos[6];

	GLuint			VBO;
	GLuint			IBO;

	GLuint			NumIndices;
	GLuint			NumIndicesMat0;

	STD_Static_Mesh		Monos;
	STD_Static_Mesh		Piso;

	STDEffect		ShaderPrograms;


	GLint			CurrentFBO;
	GLuint			DepthBuffer;
	GLuint			ReflextFBO;
	GLuint			ReflectTexture;

	GLuint			DepthFBO;
	GLuint			ShadowMapTex;

	std_uint		ShadowRes;
	std_uint		ReflectRes;
	bool			PCF;
	bool			HighQReflect;

	bool			lCamera;

	XDataBase		XMeshDataBase;
};

#endif
