/*=============================================================================
Plane.h  	
Revision history:
* Created by Camus
=============================================================================*/

#ifndef FTPLANE_H
#define FTPLANE_H
 
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <iostream>
#include <string>

using namespace std;
 
#include "../../XLoader/Include/XFileParser.h"
#include "TextureContainer.h"
#include "GlobalHeader.h"
#include "Cam.h"
#include "FTMaths.h"
 
 
 
 class Plane{
 public:
	Plane(){Indices=STDNULL;}
 	void Create(string modelPath,string texturePath);
 	void Render(Cam* cam,bool invert);
 	void Release();
 
 private:
 	Parser		MeshP;
 	FTTexture	TextureP;
	STDMATRIX	WorldP;
 	GLuint		TextureId;
 	GLuint		ShaderProgramP;
 	GLuint		VertexShaderP;
 	GLuint		FragShaderP;
 	GLuint		VertexLoc;
 	GLuint		UVLoc;
	GLuint		isampler2DP;
 	unsigned short  *Indices;

 
 	
 };

#endif