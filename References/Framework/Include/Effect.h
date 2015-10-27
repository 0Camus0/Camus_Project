/*=============================================================================
Effect.h   : Shader program manager.
Revision history:
* Created by Camus
=============================================================================*/


#ifndef EFFECTS_H
#define EFFECTS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

#include <EGL/egl.h>
#include <GLES2/gl2.h>


#include "FTMaths.h"

#include "GlobalHeader.h"

#include "../../XLoader/Include/xOGLESMesh.h"

using namespace std;


class _STDEffect;

class STDTechnique{
public:
	STDTechnique(){
		CullMode = GL_FRONT;
		FaceMode = GL_CCW;
		Program = 0;
		VertexIndex = 0;
		FragmentIndex = 0;
	}

	bool	CompileTechnique(std::string name,std::string vert,std::string pixel);
	void	ApplyRenderStates(bool enable);
	void	UseTechnique();
	void	CleanTechnique();
	std_uint GetProgram(){ return Program; }

	friend class _STDEffect;

	std_uint GetDiffuseSamplerLoc(){ return DiffSamplerloc; }
	std_uint GetReflectSamplerLoc(){ return	ReflectMapSamplerloc; }
	std_uint GetShadowSamplerLoc(){ return	ShadowMapSamplerloc; }
	std_uint GetPCFUniformLoc(){ return	UsePCFloc; }
	std_uint GetShadowMapResLoc(){ return ResShadowMaploc; }

//private:
	std::string				Name;
	std::vector<std_uint>	vCaps;
	std_uint				CullMode;
	std_uint				FaceMode;
	std_uint				Program;
	std_uint				VertexIndex;
	std_uint				FragmentIndex;

	std_uint				Pos0;
	std_uint				TexCoord0;
	std_uint				TexCoord1;
	std_uint				TexCoord2;
	std_uint				TexCoord3;
	std_uint				Normal0;
	std_uint				Tangent0;
	std_uint				Binormal0;
	std_uint				VertexWeights0;
	std_uint				VertexIndex0;

	std_uint				Wloc;
	std_uint				WVPloc;
	std_uint				WVPLightloc;
	std_uint				UsePCFloc;
	std_uint				ResShadowMaploc;
	std_uint				Camloc;
	std_uint				Light0loc;
	std_uint				Light1loc;
	std_uint				Light2loc;
	std_uint				DiffSamplerloc;
	std_uint				NormalSamplerloc;
	std_uint				SpecularSamplerloc;
	std_uint				GlossSamplerloc;
	std_uint				ShadowMapSamplerloc;
	std_uint				ReflectMapSamplerloc;
	

	std_uint				NumTextures;

};


typedef class _STDEffect{

public:
	_STDEffect(){}

	void	AddTechnique(std::string name,std::string vert,std::string pixel);
	void	SetTechnique(std::string Name);
	void	SetWorldMatrix(STDMATRIX& w);
	void	SetWVPMatrix(STDMATRIX& wvp);
	void	SetWVPLightMatrix(STDMATRIX& wvp);
	void	SetCamPos(STDVECTOR3& campos);
	void	SetLightPos(STDVECTOR3& lightpos,std_uint lightindex);
	void	SetDifusseTexture(std_uint id);
	void	SetNormalTexture(std_uint id);
	void	SetSpecularTexture(std_uint id);
	void	SetOtherTexture(std_uint id,std_uint sampler,std_uint samplernumber);
	
	void	SetFloat(std::string Name,float &f);
	void	SetVector3(std::string Name,STDVECTOR3 &v3);
	void	SetVector2(std::string Name,STDVECTOR2 &v2);
	void	SetMatrix(std::string Name,STDMATRIX &mat);
	void	SetBool(std::string Name,bool &b);
	void	SetInt(std::string Name,int &i);

	void	EnableVertexLocs(const xSubsetInfo &subset);
	void	DisableVertexLocs(const xSubsetInfo &subset);

	void	EnalbeSTD_TBNVertexLocs();
	void	DisableSTD_TBNVertexLocs();

	void	EnableSTD_TBNVeretexToDepthLocs();
	void	DisableSTD_TBNVeretexToDepthLocs();

	std::map<std::string,STDTechnique>::iterator GetActual(){ return Actual; }
//private:
	std::map<std::string,STDTechnique>				Techniques;
	std::map<std::string,STDTechnique>::iterator	 Actual;
	std::vector<STDTechnique>						vTechniques;


}STDEffect,*LPSTDEFFECT;

#endif