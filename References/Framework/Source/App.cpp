#include "../Include/GlobalHeader.h"
#include "../Include/App.h"

#include <iostream>

using namespace std;

#include "../../ThirdParty/soil/SOIL.h"

GApp::GApp()
{
	// pointers to 0
	pFrame = STDNULL;

}

void GApp::Init()
{

	// Here we setup the camera and the light camera
	Camera.SetMaxVelocity( 1200.0f );
	Camera.SetPosition( &STDVECTOR3(-2.60666f, 109.199f, 221.997f ) );
	//Camera.SetLookAt( &STDVECTOR3( -2.6155f, 108.718, 221.121) );
	Camera.SetLookAt( &STDVECTOR3( 0.0f, 0.0f, 0.0f) );
	Camera.SetNearPlane(1.0f);
	Camera.SetFarPlane(50000.0f);
	Camera.SetFOV( Deg2Rad(45.0f) );
	Camera.SetMaxPitch(  Deg2Rad(89.0f) );
	Camera.SetAspectRatio( (float)(pFrame->GetWidth()) / (float)(pFrame->GetHeight())  );
	Camera.OnUpdate(0.0f);
	LightPos = STDVECTOR3(0.0f, 203.0f, 228.0f );

	FirstFrame = false;

	LightCamera.SetMaxVelocity( 1200.0f );
	LightCamera.SetPosition( &LightPos );
	LightCamera.SetLookAt( &STDVECTOR3( 0.0, 1.0, 0.0) );
	LightCamera.SetNearPlane(1.0f);
	LightCamera.SetFarPlane(1000.0f);
	LightCamera.SetFOV( Deg2Rad(45.0f) );
	LightCamera.SetMaxPitch(  Deg2Rad(89.0f) );
	LightCamera.SetAspectRatio( (float)pFrame->GetWidth() / (float)pFrame->GetHeight()  );
	LightCamera.OnUpdate(0.0f);
	
	lCamera = false;

	// checking config.txt
	if(OglDrv::Instance()->Readfile){

		if(OglDrv::Instance()->HighQShadows){
			ShadowRes = 1024;
			PCF = true;
		}else{
			ShadowRes = 512;
			PCF = false;
		}

		if(OglDrv::Instance()->HighQReflect){
			ReflectRes = 1024;
			HighQReflect = true;
		}else{
			ReflectRes = 256;
			HighQReflect = false;
		}
	}else{
		ShadowRes = 1024;
		ReflectRes = 1024;
		PCF = true;
		HighQReflect = true;
	}


}




void GApp::OnCreate()
{
	// Loading the geometry from .x files (Directx .x model file format)
	Monos.LoadMesh("Multi.X");
	Piso.LoadMesh("Piso.X");

	

	// Set to identity the world matrix
	MatIdentity(&World);
	
	// Loading shader programs:
	// for static geometry 
	ShaderPrograms.AddTechnique("StaticNormalMesh","Shaders/Vertex.vert","Shaders/Pixel.frag");
	// for the floor (include reflection) 
	ShaderPrograms.AddTechnique("FloorMesh","Shaders/Floor.vert","Shaders/Floor.frag");
	// render to depth (shadow map)
	ShaderPrograms.AddTechnique("StatictoDepth","Shaders/Depth.vert","Shaders/Depth.frag");

	ShaderPrograms.AddTechnique("Anim","Shaders/Anim.vert","Shaders/Anim.frag");

	
	// Loading the skybox
	Planos[0].Create("Skyxneg.X","Texturas/granja x neg.png");
	Planos[1].Create("Skyxpos.X","Texturas/granja x pos.png");
	Planos[2].Create("Skyyneg.X","Texturas/granja y neg.png");
	Planos[3].Create("Skyypos.X","Texturas/granja y pos.png");
	Planos[4].Create("Skyzneg.X","Texturas/granja z neg.png");
	Planos[5].Create("Skyzpos.X","Texturas/granja z pos.png");
/*
	// creating frame buffer objects
	CreateFBORelfect();
	CreateFBOShadowMap();
*/
	// setting up the render states
	glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glClearDepthf(1.0f);

/*
	// Loading the Diavlo font
	Font = new FTBufferOptFont("Fuente/Diavlo_BLACK_II_37.otf");
	// Font size based on the screen height
	Font->FaceSize(pFrame->GetHeight()/8);
	// adjust aspect ratio
	Font->SetRect(0.0f,pFrame->GetWidth(),0.0f,pFrame->GetHeight());

	// 
	Font->SetText("FPS: ",0, 5.0f,pFrame->GetHeight() - pFrame->GetHeight()/20,0.0f,0.5,1.0f,0.6f,0.3f);
	Font->SetText("q - Salir",1, 5.0f, pFrame->GetHeight()/8,0.0f,0.5,1.0f,0.6f,0.3f);
	Font->SetText("t - ScreenShoot",2, 5.0f,pFrame->GetHeight()/4,0.0f,0.5,1.0f,0.6f,0.3f);

	if(pFrame->Windowed)
		Font->SetText("F1 - Pantalla Completa",3, 5.0f,5.0f,0.0f,0.5,1.0f,0.6f,0.3f);
	else
		Font->SetText("F1 - A Ventana",3, 5.0f,5.0f,0.0f,0.5,1.0f,0.6f,0.3f);
*/

//	XMeshDataBase.LoadXFile("HKANIMS.X");
	//XMeshDataBase.LoadXFile("dummy.X");

	XMeshDataBase.LoadXFile("DealerA14.X");
//	XMeshDataBase.LoadXFile("NuCroc.X");
//	XMeshDataBase.LoadXFile("NuVenomJok.X");
	

}
/*
To archieve planar reflections we need to render the goemetry scaled by (1,-1,1) to
an offscreen surface, so we can use it in the final pass with projective texture coordinates.
That surface will be this frame buffer object.
*/
void GApp::CreateFBORelfect(){
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFBO);

	glGenRenderbuffers(1, &DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, ReflectRes, ReflectRes );

	glGenTextures(1, &ReflectTexture);
	glBindTexture(GL_TEXTURE_2D, ReflectTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ReflectRes, ReflectRes, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	/*
	Since the plane is a big surface, linear interpolation could be a performance hit.
	*/
	if(HighQReflect){
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}else{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, &ReflextFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ReflextFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ReflectTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer);


	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
}	

/*
To archieve shadows, we use the shadow map technique, wich compare the depth of the scene from the
light perspective (stored in a offscreen surface) with the distance of the pixel to the light,
if the distance is bigger than the stored in the map, that pixel is in the shadow. We need again
a surface to store the depth, that's why we need a frame buffer object.
*/
void GApp::CreateFBOShadowMap(){
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFBO);

	glGenTextures(1, &ShadowMapTex);
	glBindTexture(GL_TEXTURE_2D, ShadowMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowRes, ShadowRes, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	

	glGenFramebuffers(1, &DepthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMapTex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
}

// cleaning up
void GApp::OnDestroy()
{
	Release();

}
// Here we update vars
void GApp::OnUpdate(float globalTime)
{
	
	Camera.SetAspectRatio( (float)pFrame->GetWidth() / (float)pFrame->GetHeight()  );
	LightCamera.SetAspectRatio( (float)pFrame->GetWidth() / (float)pFrame->GetHeight()  );
	// update cameras
	Camera.OnUpdate(globalTime);
	LightCamera.OnUpdate(globalTime);

	// update world transforms
	STDMATRIX translate,sc,rot,rotX,scinv;
	MatTranslation(&translate,0.0f,0.0f,0.0f);
	MatRotationY(&rot,Deg2Rad(180.0f));
	MatRotationX(&rotX,Deg2Rad(0.0f));
	MatScaling(&sc,1.0f,1.0f,1.0f);
	MatScaling(&scinv,1.0f,-1.0f,1.0f);
	World=sc*(rot*rotX)*translate;

	MatrizMonosInvertida = scinv*rot*translate;;
	MatrizMonosNormal=World;
	MatrizPiso=World;

	
	

	// check the time between frames to get the fps
	float ms = (1.0f/globalTime);
	static float tempTime = 0.0f;
	tempTime += globalTime;

	// force to refresh every 250 ms
	if(tempTime>.25f){
	//	Font->SetText(("FPS: " + double2string(ms)).c_str(),0, 5.0f,pFrame->GetHeight() - pFrame->GetHeight()/20,0.0f,0.5,1.0f,0.6f,0.3f);
		tempTime=0.0f;
	}

//	Font->SetText("q - Salir",1, 5.0f,pFrame->GetHeight()/8,0.0f,0.5,1.0f,0.6f,0.3f);
//	Font->SetText("t - ScreenShoot",2, 5.0f,pFrame->GetHeight()/4,0.0f,0.5,1.0f,0.6f,0.3f);
	
	//STDVECTOR3 move;
	//move.x = move.y = move.z = 0;
	//Camera.SetLookAt(&move);

	for(unsigned int i = 0; i < XMeshDataBase.XMeshDataBase.size();i++){
		xMesh* mesh = XMeshDataBase.XMeshDataBase[i];
		mesh->Update();
	}
}

// All the input goes here
void GApp::OnInput(float globalTime)
{

	static const float cameraSpeed = 75.0f;
	static const float cameraSensivity = 0.005f;
	static const float lightSpeed = 100.0f;

	// cursor delta
	int xDelta = EventManager::Instance()->GetXDiff();
	int yDelta = EventManager::Instance()->GetYDiff();

		if(FirstFrame)
		{
			if(lCamera){
				LightCamera.Yaw( cameraSensivity*xDelta);
				LightCamera.Pitch( cameraSensivity*yDelta);
			}else{
				Camera.Yaw( cameraSensivity*xDelta);
				Camera.Pitch( cameraSensivity*yDelta);
			}
               
		}

	// array of pressed keys
	bool *pPressedKeys = EventManager::Instance()->GetPressedKeys();

	// toggle to fullscreen
	if(pPressedKeys[SDLK_F1] )
	{
		EventManager::Instance()->LockKey(SDLK_F1);
		
		pFrame->Windowed = !pFrame->Windowed;

		if(pFrame->Windowed)
			SDL_SetVideoMode(pFrame->GetWidth(),pFrame->GetHeight(),32,SDL_HWSURFACE|SDL_RESIZABLE);
		else
			SDL_SetVideoMode(pFrame->GetWidth(),pFrame->GetHeight(),32,SDL_HWSURFACE|SDL_FULLSCREEN);

		if(pFrame->Windowed)
			Font->SetText("F1 - Pantalla Completa",3, 5.0f,5.0f,0.0f,0.5,1.0f,0.6f,0.3f);
		else
			Font->SetText("F1 - A Ventana",3, 5.0f,5.0f,0.0f,0.5,1.0f,0.6f,0.3f);
		
		Camera.SetAspectRatio( (float)pFrame->GetWidth() / (float)pFrame->GetHeight()  );
		
		
		
	}

	// take the screenshot
	if(pPressedKeys[SDLK_t])
	{
		EventManager::Instance()->LockKey(SDLK_t);

		SYSTEMTIME systemTime;
		GetSystemTime( &systemTime );
		string text = "Screenshot - " + double2string(systemTime.wDay) + "-" + double2string(systemTime.wMonth) + "-" + double2string(systemTime.wYear) +
					   "-" + double2string(systemTime.wHour) + "-" + double2string(systemTime.wMinute) + "-" + double2string(systemTime.wSecond)  + ".tga";
		// soil uses glReadPixels to construct the surface in rgb format 
		SOIL_save_screenshot(text.c_str(),SOIL_SAVE_TYPE_TGA,0,0,pFrame->GetWidth(),pFrame->GetHeight());
		cout << "screen tomado: " << text << endl;
	}

	// if we want to quit
	if(pPressedKeys[SDLK_q])
	{
		EventManager::Instance()->LockKey(SDLK_q);
		pPressedKeys[SDLK_q] = false;

		pFrame->Close();
	}

	xMesh* mesh = XMeshDataBase.XMeshDataBase[0];

	if(pPressedKeys[SDLK_RIGHT])
	{
		EventManager::Instance()->LockKey(SDLK_RIGHT);


		mesh->m_AnimController->NextAnimationSet();
	
	}

	if(pPressedKeys[SDLK_DOWN])
	{
		EventManager::Instance()->LockKey(SDLK_DOWN);

		mesh->m_AnimController->ResetAnimationSet();

	}

	if(pPressedKeys[SDLK_UP])
	{
		EventManager::Instance()->LockKey(SDLK_UP);

		mesh->m_AnimController->ToogleLoop();

	}

	if(pPressedKeys[SDLK_LEFT])
	{
		EventManager::Instance()->LockKey(SDLK_LEFT);
	
		mesh->m_AnimController->PrevAnimationSet();
		
	}

	if(pPressedKeys[SDLK_PAGEUP])
	{
		EventManager::Instance()->LockKey(SDLK_PAGEUP);
		mesh->m_AnimController->IncreaseSpeed();
	}

	if(pPressedKeys[SDLK_PAGEDOWN])
	{
		EventManager::Instance()->LockKey(SDLK_PAGEDOWN);
		mesh->m_AnimController->ReduceeSpeed();
	}

	if(pPressedKeys[SDLK_l])
	{
		EventManager::Instance()->LockKey(SDLK_l);
		mesh->m_AnimController->ToogleLerp();
	}



	// We can move the light
		if ( pPressedKeys[SDLK_KP8] )
		{
			LightPos.z -= lightSpeed*globalTime;
		}
		if ( pPressedKeys[SDLK_KP4] )
		{
			LightPos.x += lightSpeed*globalTime;
		}
		if ( pPressedKeys[SDLK_KP2] )
		{
			LightPos.z += lightSpeed*globalTime;
		}
		if ( pPressedKeys[SDLK_KP6] )
		{
			LightPos.x -= lightSpeed*globalTime;
		}
		if ( pPressedKeys[SDLK_KP9] )
		{
			LightPos.y += lightSpeed*globalTime;
		}

		if ( pPressedKeys[SDLK_KP7] )
		{
			LightPos.y -= lightSpeed*globalTime;
		}

		LightCamera.SetPosition(&LightPos);
		LightCamera.SetLookAt(&STDVECTOR3(0.0f,1.0f,0.0f));
		

		// to move the camera
	if(  pPressedKeys[SDLK_w] || pPressedKeys[SDLK_a] || pPressedKeys[SDLK_s] ||   pPressedKeys[SDLK_d] )
	{
		if ( pPressedKeys[SDLK_w] )
		{
			if(lCamera)
				LightCamera.MoveForward(  cameraSpeed * globalTime );
			else
				Camera.MoveForward(  cameraSpeed * globalTime );

		}
		if ( pPressedKeys[SDLK_a] )
		{
			if(lCamera)
				LightCamera.Strafe( -cameraSpeed * globalTime );
			else
				Camera.Strafe( -cameraSpeed * globalTime );
		}
		if ( pPressedKeys[SDLK_s] )
		{
			if(lCamera)
				LightCamera.MoveForward(  -cameraSpeed * globalTime );
			else
				Camera.MoveForward(  -cameraSpeed * globalTime );
		}
		if ( pPressedKeys[SDLK_d] )
		{
			if(lCamera)
				LightCamera.Strafe( cameraSpeed * globalTime );
			else
				Camera.Strafe( cameraSpeed * globalTime );
		}
	}
	else
	{

	}

}

/*
Render the scene
*/
void GApp::OnRender()
{
	// render the main pass
	glClearColor(0.6, 0.3, 0.05, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < 6 ; i++){
			Planos[i].Render(&Camera,false);
	}


	DrawMeshes();


	

/*
	// first we fill the depth
	DrawShadowMap();

	// Now that we have the depth, we can render our mirrored geometry with the correct shadow 
	DrawReflect();

	// set back our normal winding
	glFrontFace(GL_CCW);

	// render the main pass
	glClearColor(0.0, 0.0, 0.0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < 6 ; i++){
		if(lCamera)
			Planos[i].Render(&LightCamera,false);
		else
			Planos[i].Render(&Camera,false);
	}

	DrawEffectCall(&Monos,MatrizMonosNormal,MatrizMonosNormal);

	// for the floor we use and aditional sampler for the reflection
	STD_Static_Mesh *pMesh = &Piso;
	STDMATRIX Mat = MatrizPiso;
	glBindBuffer(GL_ARRAY_BUFFER, pMesh->VertexBuffObject);

	ShaderPrograms.SetTechnique("FloorMesh");

	if(PCF)
		glUniform1i(ShaderPrograms.GetActual()->second.GetPCFUniformLoc(),1);
	else
		glUniform1i(ShaderPrograms.GetActual()->second.GetPCFUniformLoc(),0);

	glUniform1f(ShaderPrograms.GetActual()->second.GetShadowMapResLoc(),(float)ShadowRes);
	ShaderPrograms.SetLightPos(LightPos,0);
	ShaderPrograms.SetLightPos(*Camera.GetPosition(),1);
	ShaderPrograms.SetCamPos(*Camera.GetPosition());
	ShaderPrograms.EnalbeSTD_TBNVertexLocs();

	for (int i = 0; i < pMesh->NumSubsets ; i++){
		STDMATRIX EndMat = pMesh->Meshes[ pMesh->Subsets[i].MeshBelong ].WorldMatrix*Mat;
		ShaderPrograms.SetWorldMatrix( EndMat );
		if(lCamera)
			ShaderPrograms.SetWVPMatrix( EndMat * ( *LightCamera.GetViewMatrix() ) * (*LightCamera.GetProjectionMatrix()) );
		else
			ShaderPrograms.SetWVPMatrix( EndMat * ( *Camera.GetViewMatrix() ) * (*Camera.GetProjectionMatrix()) );

		ShaderPrograms.SetWVPLightMatrix( EndMat * ( *LightCamera.GetViewMatrix() ) * (*LightCamera.GetProjectionMatrix()) );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->Subsets[i].Id);


		ShaderPrograms.SetDifusseTexture(pMesh->Subsets[i].Material.DiffuseId);
		ShaderPrograms.SetNormalTexture(pMesh->Subsets[i].Material.NormalId);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, ReflectTexture );
		glUniform1i(ShaderPrograms.GetActual()->second.GetReflectSamplerLoc(),2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, ShadowMapTex );
		glUniform1i(ShaderPrograms.GetActual()->second.GetShadowSamplerLoc(),3);

		glDrawElements(GL_TRIANGLES, pMesh->Subsets[i].NumTris*3, GL_UNSIGNED_SHORT,  0);
	}

	ShaderPrograms.DisableSTD_TBNVertexLocs();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

*/

//	Font->Render("");

	// swap the back buffer to display the final surface 
	eglSwapBuffers(OglDrv::Instance()->eglDisplay, OglDrv::Instance()->eglSurface);

	FirstFrame = true;
}

void GApp::DrawReflect(){
	glBindFramebuffer(GL_FRAMEBUFFER, ReflextFBO);

	glViewport(0, 0, ReflectRes, ReflectRes);

	glClearColor(0.0, 0.0, 0.0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// we need to change the winding since we scaled by -1 on one axis
	glFrontFace(GL_CW);

	// render as usual but mirrored
	for (int i = 0; i < 6 ; i++){
			Planos[i].Render(&Camera,true);
	}
	
	DrawEffectCall(&Monos,MatrizMonosInvertida,MatrizMonosNormal);

	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);

	glViewport(0, 0, pFrame->GetWidth(), pFrame->GetHeight());

}

void GApp::DrawShadowMap(){

	glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO);

	glViewport(0, 0, ShadowRes, ShadowRes);
	// we don't need color, only depth
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
	glFrontFace(GL_CCW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Render only position 
	DrawEffectCallToDepth(&Monos,MatrizMonosNormal);
	DrawEffectCallToDepth(&Piso,MatrizMonosNormal);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
	glViewport(0, 0, pFrame->GetWidth(), pFrame->GetHeight());
}

void GApp::Release()
{
	TextureContainer::Instance()->ReleaseTextures();

	Monos.Release();
	Piso.Release();
	
}

void GApp::DrawMeshes(){
	


	for(unsigned int i = 0; i < XMeshDataBase.XMeshDataBase.size();i++){
		xMesh* mesh = XMeshDataBase.XMeshDataBase[i];
		
		for(unsigned int j = 0; j < mesh->MeshInfo.size(); j++){
			glBindBuffer(GL_ARRAY_BUFFER,mesh->MeshInfo[j].Id);
			ShaderPrograms.SetTechnique("Anim");

			ShaderPrograms.SetLightPos(LightPos,0);
			ShaderPrograms.SetLightPos(*Camera.GetPosition(),1);
			ShaderPrograms.SetCamPos(*Camera.GetPosition());

			for(unsigned int k = 0; k < mesh->MeshInfo[j].Subsets.size();k++){

				xSubsetInfo *subset = &mesh->MeshInfo[j].Subsets[k];

				ShaderPrograms.EnableVertexLocs(mesh->MeshInfo[j].Subsets[k]);

				ShaderPrograms.SetWorldMatrix( MatrizMonosNormal );
				ShaderPrograms.SetWVPMatrix( MatrizMonosNormal * ( *Camera.GetViewMatrix() ) * (*Camera.GetProjectionMatrix()) );


				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subset->Id );

				glDrawElements(GL_TRIANGLES, subset->NumTris*3, GL_UNSIGNED_SHORT,  0);

				ShaderPrograms.DisableVertexLocs(mesh->MeshInfo[j].Subsets[k]);
			}
		}	
	}


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void GApp::DrawEffectCall(STD_Static_Mesh *pMesh,STDMATRIX &Mat,STDMATRIX &MatShadow){

	glBindBuffer(GL_ARRAY_BUFFER, pMesh->VertexBuffObject);

	ShaderPrograms.SetTechnique("StaticNormalMesh");

	if(PCF)
		glUniform1i(ShaderPrograms.GetActual()->second.GetPCFUniformLoc(),1);
	else
		glUniform1i(ShaderPrograms.GetActual()->second.GetPCFUniformLoc(),0);

	glUniform1f(ShaderPrograms.GetActual()->second.GetShadowMapResLoc(),(float)ShadowRes);
	ShaderPrograms.SetLightPos(LightPos,0);
	ShaderPrograms.SetLightPos(*Camera.GetPosition(),1);
	ShaderPrograms.SetCamPos(*Camera.GetPosition());
	ShaderPrograms.EnalbeSTD_TBNVertexLocs();
	
	for (int i = 0; i < pMesh->NumSubsets ; i++){
		STDMATRIX EndMat = pMesh->Meshes[ pMesh->Subsets[i].MeshBelong ].WorldMatrix*Mat;
		STDMATRIX EndMatTwo = pMesh->Meshes[ pMesh->Subsets[i].MeshBelong ].WorldMatrix*MatShadow;
		ShaderPrograms.SetWorldMatrix( EndMat );

		if(lCamera)
			ShaderPrograms.SetWVPMatrix( EndMat * ( *LightCamera.GetViewMatrix() ) * (*LightCamera.GetProjectionMatrix()) );
		else
			ShaderPrograms.SetWVPMatrix( EndMat * ( *Camera.GetViewMatrix() ) * (*Camera.GetProjectionMatrix()) );

		ShaderPrograms.SetWVPLightMatrix( EndMatTwo * ( *LightCamera.GetViewMatrix() ) * (*LightCamera.GetProjectionMatrix()) );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->Subsets[i].Id);
		ShaderPrograms.SetDifusseTexture(pMesh->Subsets[i].Material.DiffuseId);
		ShaderPrograms.SetNormalTexture(pMesh->Subsets[i].Material.NormalId);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, ShadowMapTex );
		glUniform1i(ShaderPrograms.GetActual()->second.GetShadowSamplerLoc(),2);
		glDrawElements(GL_TRIANGLES, pMesh->Subsets[i].NumTris*3, GL_UNSIGNED_SHORT,  0);
	}

	ShaderPrograms.DisableSTD_TBNVertexLocs();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GApp::DrawEffectCallToDepth(STD_Static_Mesh *pMesh,STDMATRIX &Mat){
	glBindBuffer(GL_ARRAY_BUFFER, pMesh->VertexBuffObject);
	
	ShaderPrograms.SetTechnique("StatictoDepth");
	ShaderPrograms.EnableSTD_TBNVeretexToDepthLocs();

	for (int i = 0; i < pMesh->NumMeshes ; i++){
	STDMATRIX EndMat = pMesh->Meshes[i].WorldMatrix*Mat;
	ShaderPrograms.SetWorldMatrix( EndMat );
	ShaderPrograms.SetWVPMatrix( EndMat * ( *LightCamera.GetViewMatrix() ) * (*LightCamera.GetProjectionMatrix()) );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->Meshes[i].Id);
	glDrawElements(GL_TRIANGLES, pMesh->Meshes[i].NumTris*3, GL_UNSIGNED_SHORT,  0);
	}

	ShaderPrograms.DisableSTD_TBNVeretexToDepthLocs();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// Helper function to convert std string to double
double string2double(const string &s)
{
	double number;
	stringstream ss(s);
	ss >> number;
	return number;
}

// Helper function to convert double to string 
string double2string(const double &d)
{
	ostringstream fort;
	fort << d;
	return fort.str();    
}

string double2stringlowP(const double &d)
{
	ostringstream fort;
	fort << d;
	return fort.str();    
}
