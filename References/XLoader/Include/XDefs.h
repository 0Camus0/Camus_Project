/*=============================================================================
XDefs.h 	: .X Enums and Types
Revision history:
* Created by Camus
=============================================================================*/
#ifndef XDEFS_H
#define XDEFS_H

#if defined(WIN32) || defined(_WIN32)
#pragma warning( disable : 4521 )
#pragma warning( disable : 4522 )
#endif

#include "../../Framework/Include/xMaths.h"
#include <string>
#include <vector>
#include <iostream>

namespace xF{

typedef unsigned long       stdDWORD;
typedef int                 stdBOOL;
typedef unsigned char       stdBYTE;
typedef unsigned short      stdWORD;
typedef float               stdFLOAT;
typedef std::string			stdSTRING;



typedef enum _STDEFFECTENUM{
	STDX_STRINGS = 0x1,
	STDX_FLOATS  = 0x2,
	STDX_DWORDS  = 0x3, 
} STDEFFECTENUM;


typedef enum _STDDECLMETHOD {
	STDDECLMETHOD_DEFAULT            = 0,
	STDDECLMETHOD_PARTIALU           = 1,
	STDDECLMETHOD_PARTIALV           = 2,
	STDDECLMETHOD_CROSSUV            = 3,
	STDDECLMETHOD_UV                 = 4,
	STDDECLMETHOD_LOOKUP             = 5,
	STDDECLMETHOD_LOOKUPPRESAMPLED   = 6 
} STDDECLMETHOD;

typedef enum _STDDECLTYPE {
	STDDECLTYPE_FLOAT1      = 0,
	STDDECLTYPE_FLOAT2      = 1,
	STDDECLTYPE_FLOAT3      = 2,
	STDDECLTYPE_FLOAT4      = 3,
	STDDECLTYPE_STDCOLOR    = 4,
	STDDECLTYPE_UBYTE4      = 5,
	STDDECLTYPE_SHORT2      = 6,
	STDDECLTYPE_SHORT4      = 7,
	STDDECLTYPE_UBYTE4N     = 8,
	STDDECLTYPE_SHORT2N     = 9,
	STDDECLTYPE_SHORT4N     = 10,
	STDDECLTYPE_USHORT2N    = 11,
	STDDECLTYPE_USHORT4N    = 12,
	STDDECLTYPE_UDEC3       = 13,
	STDDECLTYPE_DEC3N       = 14,
	STDDECLTYPE_FLOAT16_2   = 15,
	STDDECLTYPE_FLOAT16_4   = 16,
	STDDECLTYPE_UNUSED      = 17 
} STDDECLTYPE;

typedef enum _STDDECLUSAGE {
	STDDECLUSAGE_POSITION       = 0,
	STDDECLUSAGE_BLENDWEIGHT    = 1,
	STDDECLUSAGE_BLENDINDICES   = 2,
	STDDECLUSAGE_NORMAL         = 3,
	STDDECLUSAGE_PSIZE          = 4,
	STDDECLUSAGE_TEXCOORD       = 5,
	STDDECLUSAGE_TANGENT        = 6,
	STDDECLUSAGE_BINORMAL       = 7,
	STDDECLUSAGE_TESSFACTOR     = 8,
	STDDECLUSAGE_POSITIONT      = 9,
	STDDECLUSAGE_COLOR          = 10,
	STDDECLUSAGE_FOG            = 11,
	STDDECLUSAGE_DEPTH          = 12,
	STDDECLUSAGE_SAMPLE         = 13 
} STDDECLUSAGE;

enum XReturnType
{
	STD_FILE_NOT_FOUND,
	STD_INVALID_FILE,
	STD_UNKNOWN_FORMAT,
	STD_OK
};

enum XTemplateType{
	STD_X_TEMPLATE = 0,
	STD_X_KEYVALUEPAIR,
	STD_X_FRAME,
	STD_X_FRAME_TRANSFORM_MTX,
	STD_X_OBJ_CMMTX,
	STD_X_MESH,
	STD_X_MESH_NORMALS,
	STD_X_MESH_TEXCOORD,
	STD_X_DECLDATA,
	STD_X_SKIN_HEADER,
	STD_X_SKIN_WEIGHTS,
	STD_X_MATERIALS_LIST,
	STD_X_MATERIAL,
	STD_X_EFFECT_INSTANCE,
	STD_X_EFFECT_PARAMS_DWORD,
	STD_X_EFFECT_PARAMS_FLOAT,
	STD_X_EFFECT_PARAMS_STRING,
	STD_X_TEXTURE,
	STD_X_TICKSPERSECCOND,
	STD_X_ANIMATIONSET,
	STD_X_ANIMATION,
	STD_X_ANIMATION_KEY,
	STD_X_ANIMATION_OPTIONS,
	STD_X_REF,
};

const std::string XstringTemplates[] = {
	"template ",
	"KeyValuePair ",
	"Frame ",
	"FrameTransformMatrix ",
	"ObjectMatrixComment ",
	"Mesh ",
	"MeshNormals ",
	"MeshTextureCoords ",
	"DeclData ",
	"XSkinMeshHeader ",
	"SkinWeights ",
	"MeshMaterialList ",
	"Material ",
	"EffectInstance ",
	"EffectParamDWord ",
	"EffectParamFloats ",
	"EffectParamString ",
	"TextureFilename ",
	"AnimTicksPerSecond ",
	"AnimationSet ",
	"Animation ",
	"AnimationKey ",
	"AnimationOptions "
};

struct xKeyValuePair{
	std::string Key;
	std::string Value;
};

struct xMeshFace{
	xMeshFace(){ NumVertexIndices = 0; Indices = xNULL; }

	xMeshFace(const xMeshFace &mf){ *this = mf; }
	xMeshFace(xMeshFace &mf){ *this = mf; }
	xMeshFace & operator= (const xMeshFace & other){
		this->NumVertexIndices = other.NumVertexIndices;
		Indices = new stdWORD[this->NumVertexIndices];
		for (unsigned int i = 0; i < this->NumVertexIndices ; i++){
			Indices[i] = other.Indices[i];
		}
		return *this;
	}
	xMeshFace & operator= (xMeshFace & other){
		this->NumVertexIndices = other.NumVertexIndices;
		Indices = new stdWORD[this->NumVertexIndices];
		for (unsigned int i = 0; i < this->NumVertexIndices ; i++){
			Indices[i] = other.Indices[i];
		}
		return *this;
	}

	~xMeshFace(){ 
		delete [] Indices; 
		NumVertexIndices = 0;
		Indices = 0;
	}
	stdDWORD	NumVertexIndices;
	stdWORD*	Indices;
};

struct xTexCoords{
	xTexCoords(){ NumTexcoords = 0; TexCoordinates = xNULL;}
	xTexCoords(const xTexCoords &tc){ *this = tc; }
	xTexCoords(xTexCoords &tc){ *this = tc; }
	xTexCoords & operator= (const xTexCoords & other){
		this->NumTexcoords = other.NumTexcoords;
		TexCoordinates = new XVECTOR2[this->NumTexcoords];
		for (unsigned int i = 0; i < this->NumTexcoords ; i++){
			TexCoordinates[i] = other.TexCoordinates[i];
		}
		return *this;
	}
	xTexCoords & operator= (xTexCoords & other){
		this->NumTexcoords = other.NumTexcoords;
		TexCoordinates = new XVECTOR2[this->NumTexcoords];
		for (unsigned int i = 0; i < this->NumTexcoords ; i++){
			TexCoordinates[i] = other.TexCoordinates[i];
		}
		return *this;
	}

	~xTexCoords(){ 	
		NumTexcoords = 0;
		delete [] TexCoordinates;
		TexCoordinates = 0;
	}
	stdDWORD		 NumTexcoords;
	XVECTOR2		*TexCoordinates;
};

struct xVertices{
	xVertices():NumVertices(0),Vertices(0){}
	xVertices(const xVertices &tc){ *this = tc; }
	xVertices(xVertices &v3){ *this = v3; }
	xVertices & operator= (const xVertices & other){
		this->NumVertices = other.NumVertices;
		Vertices = new XVECTOR3[this->NumVertices];
		for (unsigned int i = 0; i < this->NumVertices ; i++){
			Vertices[i] = other.Vertices[i];
		}
		return *this;
	}
	xVertices & operator= (xVertices & other){
		this->NumVertices = other.NumVertices;
		Vertices = new XVECTOR3[this->NumVertices];
		for (unsigned int i = 0; i < this->NumVertices ; i++){
			Vertices[i] = other.Vertices[i];
		}
		return *this;
	}
	~xVertices(){ 	
			NumVertices = 0;
			delete [] Vertices;
			Vertices = 0;

	}
	stdDWORD	NumVertices;
	XVECTOR3	*Vertices;
};

struct xGeometry{
	xGeometry(){ NumVertices = NumFaces = 0; Vertices = xNULL; Faces = xNULL; }

	xGeometry(const xGeometry &tc){ *this = tc; }

	xGeometry(xGeometry &v3){ *this = v3; }

	xGeometry & operator= (const xGeometry & other){
		this->NumVertices = other.NumVertices;
		this->NumFaces = other.NumFaces;
		Vertices = new XVECTOR3[this->NumVertices];
		Faces = new xMeshFace[this->NumFaces];
		for (unsigned int i = 0; i < this->NumVertices ; i++){
			Vertices[i] = other.Vertices[i];
		}
		for (unsigned int i = 0; i < this->NumFaces ; i++){
			Faces[i] = other.Faces[i];
		}
		return *this;
	}

	xGeometry & operator= (xGeometry & other){
		this->NumVertices = other.NumVertices;
		this->NumFaces = other.NumFaces;
		Vertices = new XVECTOR3[this->NumVertices];
		Faces = new xMeshFace[this->NumFaces];
		for (unsigned int i = 0; i < this->NumVertices ; i++){
			Vertices[i] = other.Vertices[i];
		}
		for (unsigned int i = 0; i < this->NumFaces ; i++){
			Faces[i] = other.Faces[i];
		}
		return *this;
	}
	~xGeometry(){ 
		NumVertices = 0;
		NumFaces = 0;
		delete [] Vertices;		Vertices = 0;
		delete [] Faces;	Faces = 0; }
	stdDWORD		 NumVertices;
	XVECTOR3		*Vertices;
	stdDWORD		 NumFaces;
	xMeshFace		*Faces;
};

struct xVertexElement{
	xVertexElement(){ Type = Method = Usage = UsageIndex = 0; }
	stdDWORD Type;
	stdDWORD Method;
	stdDWORD Usage;
	stdDWORD UsageIndex;
};

struct xDeclData{
	xDeclData(){ NumElements = NumValues = 0; Elements = xNULL; Values = xNULL; }

	xDeclData(const xDeclData &dd){ *this = dd; }

	xDeclData(xDeclData &dd){ *this = dd; }

	xDeclData & operator= (const xDeclData & other){
		this->NumElements = other.NumElements;
		this->NumValues = other.NumValues;
		Elements = new xVertexElement[this->NumElements];
		Values = new stdDWORD[this->NumValues];
		for (unsigned int i = 0; i < this->NumElements ; i++){
			Elements[i] = other.Elements[i];
		}
		for (unsigned int i = 0; i < this->NumValues ; i++){
			Values[i] = other.Values[i];
		}
		return *this;
	}

	xDeclData & operator= (xDeclData & other){
		this->NumElements = other.NumElements;
		this->NumValues = other.NumValues;
		Elements = new xVertexElement[this->NumElements];
		Values = new stdDWORD[this->NumValues];
		for (unsigned int i = 0; i < this->NumElements ; i++){
			Elements[i] = other.Elements[i];
		}
		for (unsigned int i = 0; i < this->NumValues ; i++){
			Values[i] = other.Values[i];
		}
		return *this;
	}

	~xDeclData(){ 
		NumElements = 0;
		NumValues = 0;
		delete [] Elements; Elements = 0;
		delete [] Values; Values = 0; }

	stdDWORD			 NumElements;
	xVertexElement		*Elements;
	stdDWORD			 NumValues;
	stdDWORD			*Values;
};

struct xSkinMeshHeader{
	xSkinMeshHeader(){ MaxNumWeightPerVertex = MaxNumWeightPerFace = NumBones = NumBonesProcess = 0; }
	xSkinMeshHeader(const xSkinMeshHeader &rk){ *this = rk; }
	xSkinMeshHeader(xSkinMeshHeader &rk){ *this = rk; }
	xSkinMeshHeader & operator= (const xSkinMeshHeader & other){
		this->MaxNumWeightPerVertex = other.MaxNumWeightPerVertex;
		this->MaxNumWeightPerFace = other.MaxNumWeightPerFace;
		this->NumBones = other.NumBones;
		this->NumBonesProcess = other.NumBonesProcess;
		return *this;
	}
	xSkinMeshHeader & operator= (xSkinMeshHeader & other){
		this->MaxNumWeightPerVertex = other.MaxNumWeightPerVertex;
		this->MaxNumWeightPerFace = other.MaxNumWeightPerFace;
		this->NumBones = other.NumBones;
		this->NumBonesProcess = other.NumBonesProcess;
		return *this;
	}

	stdWORD	MaxNumWeightPerVertex;
	stdWORD	MaxNumWeightPerFace;
	stdWORD NumBones;
	stdWORD NumBonesProcess;
};

struct xSkinWeights{
	xSkinWeights(){ NumWeights = 0; VertexIndices = xNULL; Weights = xNULL; }

	xSkinWeights(const xSkinWeights &sw){ *this = sw; }

	xSkinWeights(xSkinWeights &sw){ *this = sw; }

	xSkinWeights & operator= (const xSkinWeights & other){
		this->NodeName = other.NodeName;
		for (int i = 0; i < 16 ; i++){

			this->MatrixFinal.mat[i] = other.MatrixFinal.mat[i];
			this->MatrixOffset.mat[i] = other.MatrixOffset.mat[i];
		}
		this->MatrixCombined = other.MatrixCombined;
		this->MatrixCombinedAnimation = other.MatrixCombinedAnimation;
		this->NumWeights = other.NumWeights;
		VertexIndices = new stdDWORD[other.NumWeights];
		Weights = new stdFLOAT[other.NumWeights];
		for (unsigned int i = 0; i < other.NumWeights ; i++){
			VertexIndices[i] = other.VertexIndices[i];
			Weights[i] = other.Weights[i];
		}
		return *this;
	}

	xSkinWeights & operator= (xSkinWeights & other){
		this->NodeName = other.NodeName;
		for (int i = 0; i < 16 ; i++){
			
			this->MatrixFinal.mat[i] = other.MatrixFinal.mat[i];
			this->MatrixOffset.mat[i] = other.MatrixOffset.mat[i];
		}
		this->MatrixCombined = other.MatrixCombined;
		this->MatrixCombinedAnimation = other.MatrixCombinedAnimation;
		this->NumWeights = other.NumWeights;
		VertexIndices = new stdDWORD[other.NumWeights];
		Weights = new stdFLOAT[other.NumWeights];
		for (unsigned int i = 0; i < other.NumWeights ; i++){
			VertexIndices[i] = other.VertexIndices[i];
			Weights[i] = other.Weights[i];
		}
		return *this;
	}

	~xSkinWeights() { 
		NumWeights = 0;
		delete [] VertexIndices;  VertexIndices = 0;
		delete [] Weights; Weights = 0;  MatrixCombined = 0; }
	stdDWORD	 *VertexIndices;
	stdFLOAT	 *Weights;
	XMATRIX44	  MatrixFinal;
	XMATRIX44	  MatrixOffset;
	XMATRIX44	  *MatrixCombined;	
	XMATRIX44	  *MatrixCombinedAnimation;	
	stdDWORD	  NumWeights;
	std::string	  NodeName;
	
};

struct xSkinInfo{
	xSkinInfo(){ SkinWeights = xNULL; }

	xSkinInfo(const xSkinInfo &si){ *this = si; }
	xSkinInfo(xSkinInfo &si){ *this = si; }
	xSkinInfo & operator= (const xSkinInfo & other){
		this->SkinMeshHeader.NumBones = other.SkinMeshHeader.NumBones;
		this->SkinWeights = new xSkinWeights[other.SkinMeshHeader.NumBones ];
		for (unsigned int i = 0; i < other.SkinMeshHeader.NumBones  ; i++){
			SkinWeights[i] = other.SkinWeights[i];
		}
		return *this;
	}
	xSkinInfo & operator= (xSkinInfo & other){
		this->SkinMeshHeader.NumBones = other.SkinMeshHeader.NumBones;
		this->SkinWeights = new xSkinWeights[other.SkinMeshHeader.NumBones];
		for (unsigned int i = 0; i < other.SkinMeshHeader.NumBones ; i++){
			this->SkinWeights[i] = other.SkinWeights[i];
		}
		return *this;
	}

	~xSkinInfo(){ 
		delete [] SkinWeights;  SkinWeights = 0;
	}
	xSkinMeshHeader	 SkinMeshHeader;
	xSkinWeights	*SkinWeights;
};

struct xFloatKeys{
	xFloatKeys(){ NumValues = 0; Values = xNULL; }

	xFloatKeys(const xFloatKeys &fk){ *this = fk; }
	xFloatKeys(xFloatKeys &fk){ *this = fk; }
	xFloatKeys & operator= (const xFloatKeys & other){
		this->NumValues = other.NumValues;
		Values = new stdFLOAT[this->NumValues ];
		for (unsigned int i = 0; i < this->NumValues ; i++){
			Values[i] = other.Values[i];
		}
		return *this;
	}
	xFloatKeys & operator= (xFloatKeys & other){
		this->NumValues = other.NumValues;
		Values = new stdFLOAT[this->NumValues ];
		for (unsigned int i = 0; i < this->NumValues ; i++){
			Values[i] = other.Values[i];
		}
		return *this;
	}

	~xFloatKeys(){ delete [] Values; NumValues = 0; Values = 0; }

	stdDWORD	     NumValues;
	stdFLOAT	    *Values;
};

struct xTimedFloatKeys { 
	xTimedFloatKeys(){ Time = 0; }
	stdDWORD		Time; 
	xFloatKeys		FloatKeys; 
};

struct xAnimationKey{
	xAnimationKey(){ Type = NumKeys = xNULL; Keys = xNULL; }

	xAnimationKey(const xAnimationKey &ak){ *this = ak; }
	xAnimationKey(xAnimationKey &ak){ *this = ak; }
	xAnimationKey & operator= (const xAnimationKey & other){
		this->Type = other.Type;
		this->NumKeys = other.NumKeys;
		Keys = new xTimedFloatKeys[this->NumKeys ];
		for (unsigned int i = 0; i < this->NumKeys ; i++){
			Keys[i] = other.Keys[i];
		}
		return *this;
	}
	xAnimationKey & operator= (xAnimationKey & other){
		this->Type = other.Type;
		this->NumKeys = other.NumKeys;
		Keys = new xTimedFloatKeys[this->NumKeys ];
		for (unsigned int i = 0; i < this->NumKeys ; i++){
			Keys[i] = other.Keys[i];
		}
		return *this;
	}

	~xAnimationKey(){delete [] Keys;  NumKeys = 0; Keys = 0;}

	stdDWORD		  Type;
	stdDWORD		  NumKeys;
	xTimedFloatKeys  *Keys;
};

struct xRotationKey{
	xRotationKey(){t.f_atTime=0.0f;}

	xRotationKey(const xRotationKey &rk){ *this = rk; }
	xRotationKey(xRotationKey &rk){ *this = rk; }
	xRotationKey & operator= (const xRotationKey & other){
		this->t.f_atTime = other.t.f_atTime;
		this->Rot = other.Rot;
		return *this;
	}
	xRotationKey & operator= (xRotationKey & other){
		this->t.f_atTime = other.t.f_atTime;
		this->Rot = other.Rot;
		return *this;
	}

	~xRotationKey(){t.f_atTime=0.0f;}

	union{
		unsigned int	i_atTime;
		float			f_atTime;
	}t;
	XQUATERNION		Rot;
};

struct xScaleKey{
	xScaleKey(){t.f_atTime=0.0f;}

	xScaleKey(const xScaleKey &sk){ *this = sk; }
	xScaleKey(xScaleKey &sk){ *this = sk; }
	xScaleKey & operator= (const xScaleKey & other){
		this->t.f_atTime = other.t.f_atTime;
		this->Scale = other.Scale;
		return *this;
	}
	xScaleKey & operator= (xScaleKey & other){
		this->t.f_atTime = other.t.f_atTime;
		this->Scale = other.Scale;
		return *this;
	}

	~xScaleKey(){t.f_atTime=0.0f;}

	union{
		unsigned int	i_atTime;
		float			f_atTime;
	}t;
	XVECTOR3		Scale;
};

struct xPositionKey{
	xPositionKey(){t.f_atTime=0.0f;}

	xPositionKey(const xPositionKey &pos){ *this = pos; }
	xPositionKey(xPositionKey &pos){ *this = pos; }
	xPositionKey & operator= (const xPositionKey & other){
		this->t.f_atTime = other.t.f_atTime;
		this->Position = other.Position;
		return *this;
	}
	xPositionKey & operator= (xPositionKey & other){
		this->t.f_atTime = other.t.f_atTime;
		this->Position = other.Position;
		return *this;
	}

	~xPositionKey(){t.f_atTime=0.0f;}

	union{
		unsigned int	i_atTime;
		float			f_atTime;
	}t;
	XVECTOR3		Position;
};

struct xAnimation{
	std::string			Name;
	std::string			BoneName;
	xAnimationKey		Rot;
	xAnimationKey		Pos;
	xAnimationKey		Sca;
	xAnimationKey		Mtx;
};

struct xAnimationSet_legacy{
	xAnimationSet_legacy(){ NumBones = 0; BoneAnimations = xNULL; }

	xAnimationSet_legacy(const xAnimationSet_legacy &as){ *this = as; }
	xAnimationSet_legacy(xAnimationSet_legacy &as){ *this = as; }
	xAnimationSet_legacy & operator= (const xAnimationSet_legacy & other){
		this->Name = other.Name;
		this->NumBones = other.NumBones;
		BoneAnimations = new xAnimation[this->NumBones ];
		for (unsigned int i = 0; i < this->NumBones ; i++){
			BoneAnimations[i] = other.BoneAnimations[i];
		}
		return *this;
	}
	xAnimationSet_legacy & operator= (xAnimationSet_legacy & other){
		this->NumBones = other.NumBones;
		BoneAnimations = new xAnimation[this->NumBones ];
		for (unsigned int i = 0; i < this->NumBones ; i++){
			BoneAnimations[i] = other.BoneAnimations[i];
		}
		return *this;
	}

	~xAnimationSet_legacy(){delete [] BoneAnimations; NumBones = 0; BoneAnimations = 0; }
	std::string			Name;
	stdDWORD		NumBones;
	xAnimation	   *BoneAnimations;	
};

struct STDRGBAColor {
	STDRGBAColor()
	{
		r = g = b = a = 0.0f;
	}

	STDRGBAColor(const float vr, const float vg, const float vb, const float v_a)
	{
		r = vr; g = vg; b = vb; a = v_a;
	}

	stdFLOAT r,g,b,a;
	
};

struct xEffectDefault{
	xEffectDefault(){ NumElements = 0; CaseDWORD = 0;  CaseFloat = xNULL; }

	xEffectDefault(const xEffectDefault &ed){ *this = ed; }
	xEffectDefault(xEffectDefault &ed){ *this = ed; }
	xEffectDefault & operator= (const xEffectDefault & other){
		this->NumElements = other.NumElements;
		this->NameParam = other.NameParam;
		this->CaseString = other.CaseString;
		this->Type = other.Type;
		this->CaseDWORD = other.CaseDWORD;
		CaseFloat = new stdFLOAT[this->NumElements ];
		for (unsigned int i = 0; i < this->NumElements ; i++){
			CaseFloat[i] = other.CaseFloat[i];
		}
		return *this;
	}
	xEffectDefault & operator= (xEffectDefault & other){
		this->NumElements = other.NumElements;
		this->NameParam = other.NameParam;
		this->CaseString = other.CaseString;
		this->Type = other.Type;
		this->CaseDWORD = other.CaseDWORD;
		CaseFloat = new stdFLOAT[this->NumElements ];
		for (unsigned int i = 0; i < this->NumElements ; i++){
			CaseFloat[i] = other.CaseFloat[i];
		}
		return *this;
	}


	~xEffectDefault(){ 
		NumElements = 0;
		delete [] CaseFloat;
		CaseFloat = 0;
	}
	std::string		NameParam;
	std::string		CaseString;
	STDEFFECTENUM	Type;
	stdDWORD		NumElements;
	stdDWORD		CaseDWORD;
	stdFLOAT*		CaseFloat;
	

};

struct xEffectInstance
{
	xEffectInstance(){ NumDefaults = 0; NumProcess = 0; pDefaults = xNULL; }

	xEffectInstance(const xEffectInstance &ei){ *this = ei; }
	xEffectInstance(xEffectInstance &ei){ *this = ei; }
	xEffectInstance & operator= (const xEffectInstance & other){
		this->ShaderFileName = other.ShaderFileName;
		this->NumDefaults = other.NumDefaults;
		this->NumProcess = other.NumProcess;

		pDefaults = new xEffectDefault[this->NumDefaults];
		for (unsigned int i = 0; i < this->NumDefaults ; i++){
			pDefaults[i] = other.pDefaults[i];
		}
		return *this;
	}
	xEffectInstance & operator= (xEffectInstance & other){
		this->ShaderFileName = other.ShaderFileName;
		this->NumDefaults = other.NumDefaults;
		this->NumProcess = other.NumProcess;

		pDefaults = new xEffectDefault[this->NumDefaults];
		for (unsigned int i = 0; i < this->NumDefaults ; i++){
			pDefaults[i] = other.pDefaults[i];
		}
		return *this;
	}

	~xEffectInstance(){ 
			NumDefaults = 0;
			delete [] pDefaults; 
			pDefaults = 0;
	}
	std::string		 ShaderFileName;
	stdDWORD		 NumDefaults;
	stdDWORD		 NumProcess;
	xEffectDefault	*pDefaults;
};

struct xMaterial
{
	xMaterial(){ Power = 0.0f; bEffects = false; }
	~xMaterial(){ }
	std::string			Name;
	STDRGBAColor		FaceColor;
	stdFLOAT			Power;
	STDRGBAColor		Specular;
	STDRGBAColor		Emissive;
	stdBOOL				bEffects;
	xEffectInstance		EffectInstance;
};

struct xMaterialList
{
	xMaterialList(){ NumMaterials = NumFaceIndices = 0; NumMatProcess = 0; FaceIndices = 0; Materials = 0; }

	xMaterialList(const xMaterialList &ml){ *this = ml; }
	xMaterialList(xMaterialList &ml){ *this = ml; }
	xMaterialList & operator= (const xMaterialList & other){
		this->NumMaterials = other.NumMaterials;
		this->NumFaceIndices = other.NumFaceIndices;
		this->NumMatProcess = other.NumMatProcess;

		FaceIndices = new stdDWORD[this->NumFaceIndices];
		Materials = new xMaterial[this->NumMaterials];
		for (unsigned int i = 0; i < this->NumFaceIndices ; i++){
			FaceIndices[i] = other.FaceIndices[i];
		}
		for (unsigned int i = 0; i < this->NumMaterials ; i++){
			Materials[i] = other.Materials[i];
		}
		return *this;
	}
	xMaterialList & operator= (xMaterialList & other){
		this->NumMaterials = other.NumMaterials;
		this->NumFaceIndices = other.NumFaceIndices;
		this->NumMatProcess = other.NumMatProcess;

		FaceIndices = new stdDWORD[this->NumFaceIndices];
		Materials = new xMaterial[this->NumMaterials];
		for (unsigned int i = 0; i < this->NumFaceIndices ; i++){
			FaceIndices[i] = other.FaceIndices[i];
		}
		for (unsigned int i = 0; i < this->NumMaterials ; i++){
			Materials[i] = other.Materials[i];
		}
		return *this;
	}

	~xMaterialList(){ 
		NumMaterials = 0;
		NumFaceIndices = 0;
		delete [] FaceIndices;	FaceIndices = 0;
		delete [] Materials;	Materials = 0;
	}
	stdDWORD		 NumMaterials;
	stdDWORD		 NumFaceIndices;
	stdDWORD		 NumMatProcess;
	stdDWORD		*FaceIndices;
	xMaterial		*Materials;
};

struct xBone{
	xBone():Dad(0),Touched(0){}

	xBone(const xBone &Bn){ *this = Bn; }
	xBone(xBone &Bn){ *this = Bn; }
	xBone & operator= (const xBone & other){
		for (int i = 0; i < 16 ; i++){
			this->Bone.mat[i] = other.Bone.mat[i] ;
			this->Combined.mat[i] = other.Combined.mat[i];
		}

		this->Name = other.Name;
		this->Dad = other.Dad;
		this->Touched = other.Touched;

		for (unsigned int i = 0; i < other.Brothers.size(); i++){
			Brothers.push_back(other.Brothers[i]);
		}
		for (unsigned int i = 0; i < other.Sons.size(); i++){
			Sons.push_back(other.Sons[i]);
		}
		return *this;
	}
	xBone & operator= (xBone & other){
		for (int i = 0; i < 16 ; i++){
			this->Bone.mat[i] = other.Bone.mat[i] ;
			this->Combined.mat[i] = other.Combined.mat[i];
		}

		this->Name = other.Name;
		this->Dad = other.Dad;
		this->Touched = other.Touched;

		for (unsigned int i = 0; i < other.Brothers.size(); i++){
			Brothers.push_back(other.Brothers[i]);
		}
		for (unsigned int i = 0; i < other.Sons.size(); i++){
			Sons.push_back(other.Sons[i]);
		}
		return *this;
	}


	XMATRIX44					Bone;
	XMATRIX44					Combined;
//	char						Name[64];
	std::string					Name;
	unsigned short				Dad;
	unsigned short				Touched;
	std::vector<unsigned int>	Brothers;
	std::vector<unsigned int>	Sons;
};

struct xSkeleton{
	xSkeleton():NumBones(0){}
	xSkeleton(const xSkeleton &Bn){ *this = Bn; }
	xSkeleton(xSkeleton &Bn){ *this = Bn; }
	xSkeleton & operator= (const xSkeleton & other){
		this->NumBones = other.NumBones;
		for (int i = 0; i < other.Bones.size() ; i++){
			this->Bones.push_back(other.Bones[i]);
		}
		return *this;
	}
	xSkeleton & operator= (xSkeleton & other){
		this->NumBones = other.NumBones;
		for (int i = 0; i < other.Bones.size() ; i++){
			this->Bones.push_back(other.Bones[i]);
		}
		return *this;
	}

	unsigned int						NumBones;
	std::vector<xBone>					Bones;
};

struct xAnimationSingleKey{
	enum{ START, RUNNING, FINISHED};

	xAnimationSingleKey():StatePos(START),StateRot(START),StateSc(START){}

	xPositionKey	PositionKey;
	xRotationKey	RotationKey;
	xScaleKey		ScaleKey;

	double						LocaltimePos;
	double						LocaltimeRot;
	double						LocaltimeSc;

	double						LocaltimePosLerp;
	double						LocaltimeRotLerp;
	double						LocaltimeScLerp;

	unsigned int				LocalIndexPos;
	unsigned int				LocalIndexRot;
	unsigned int				LocalIndexSc;

	unsigned int				MaxIndexPos;
	unsigned int				MaxIndexRot;
	unsigned int				MaxIndexSc;

	unsigned int				StatePos;
	unsigned int				StateRot;
	unsigned int				StateSc;
};

struct xAnimationBone{
	xAnimationBone():BoneID(0){}
	

	std::vector<xPositionKey>	PositionKeys;
	std::vector<xRotationKey>	RotationKeys;
	std::vector<xScaleKey>		ScaleKeys;
	xAnimationSingleKey			ActualKey;
	XMATRIX44					MatrixfromKeys;
	unsigned int				BoneID;	
	std::string					BoneName;
};

struct xAnimationSet{
	std::string					Name;
	std::vector<xAnimationBone> BonesRef;

	double						m_LocalTime;
	unsigned int				m_LocalIndex;
	long						m_MaxTimeOnTicks;
	long						m_NumofKeyframes;
};

struct xAnimationInfo{
	xAnimationInfo():TicksPerSecond(0),isAnimInfo(false){}
	std::vector<xAnimationSet>	Animations;
	unsigned int				TicksPerSecond;
	bool						isAnimInfo;
};



struct xMeshGeometry{
	enum{
		HAS_POSITION			= 1,
		HAS_NORMAL				= HAS_POSITION << 1,
		HAS_TANGENT				= HAS_POSITION << 2,
		HAS_BINORMAL			= HAS_POSITION << 3,
		HAS_TEXCOORD0			= HAS_POSITION << 4,
		HAS_TEXCOORD1			= HAS_POSITION << 5,
		HAS_TEXCOORD2			= HAS_POSITION << 6,
		HAS_TEXCOORD3			= HAS_POSITION << 7,
		HAS_VERTEXCOLOR			= HAS_POSITION << 8,
		HAS_SKINWEIGHTS0		= HAS_POSITION << 9,
		HAS_SKINWEIGHTS1		= HAS_POSITION << 10,
		HAS_SKININDEXES0		= HAS_POSITION << 11,
		HAS_SKININDEXES1		= HAS_POSITION << 12,
		HAS_CUSTOM_DATA_4BYTES	= HAS_POSITION << 13,
		HAS_CUSTOM_DATA_8BYTES	= HAS_POSITION << 14,
		HAS_CUSTOM_DATA_16BYTES	= HAS_POSITION << 15,
	};
	xMeshGeometry():NumChannelsTexCoords(0),NumVertices(0),NumTriangles(0),NumIndices(0),VertexSize(0),VertexAttributes(HAS_POSITION){}
	xMeshGeometry(const xMeshGeometry &mesh){ *this = mesh; }
	xMeshGeometry(const xSkeleton &mesh){ *this = mesh; }
	xMeshGeometry(xSkeleton &mesh){ *this = mesh; }
	xMeshGeometry & operator= (const xMeshGeometry & other){
		this->Positions = other.Positions;
		this->Normals = other.Normals;
		this->Tangents = other.Tangents;
		this->Binormals = other.Binormals;
		this->VertexColors = other.VertexColors;
		for (int i = 0; i < 4 ; i++){
			this->TexCoordinates[i] = other.TexCoordinates[i];
		}
		this->Triangles = other.Triangles;
		this->Info = other.Info;
		this->Materials = other.Materials;
		this->RelativeMatrix = other.RelativeMatrix;
		this->Name = other.Name;
		this->NumVertices = other.NumVertices;
		this->NumTriangles = other.NumTriangles;
		this->NumIndices = other.NumIndices;
		this->VertexSize = other.VertexSize;
		this->NumTriangles = other.NumTriangles;
		this->VertexAttributes = other.VertexAttributes;
		this->NumChannelsTexCoords = other.NumChannelsTexCoords;
		return *this;
	}
	xMeshGeometry & operator= (xMeshGeometry & other){
		this->Positions = other.Positions;
		this->Normals = other.Normals;
		this->Tangents = other.Tangents;
		this->Binormals = other.Binormals;
		this->VertexColors = other.VertexColors;
		for (int i = 0; i < 4 ; i++){
			this->TexCoordinates[i] = other.TexCoordinates[i];
		}
		this->Triangles = other.Triangles;
		this->Info = other.Info;
		this->Materials = other.Materials;
		this->RelativeMatrix = other.RelativeMatrix;
		this->Name = other.Name;
		this->NumVertices = other.NumVertices;
		this->NumTriangles = other.NumTriangles;
		this->NumIndices = other.NumIndices;
		this->VertexSize = other.VertexSize;
		this->NumTriangles = other.NumTriangles;
		this->VertexAttributes = other.VertexAttributes;
		this->NumChannelsTexCoords = other.NumChannelsTexCoords;
		return *this;
	}

	~xMeshGeometry(){}
	
	xVertices				 Positions;
	xVertices				 Normals;
	xVertices				 Tangents;
	xVertices				 Binormals;
	xVertices				 VertexColors;
	xTexCoords				 TexCoordinates[4];				
	
	xMeshFace				 Triangles;
	xSkinInfo				 Info;
	xMaterialList			 Materials;
	
	XMATRIX44				 RelativeMatrix;
	std::string				 Name;
	stdDWORD				 NumVertices;
	stdDWORD				 NumTriangles;
	stdDWORD				 NumIndices;
	stdDWORD				 VertexSize;
	stdDWORD				 VertexAttributes;
	stdWORD					 NumChannelsTexCoords;	
};

struct xMeshContainer{

	xMeshContainer(){}
	xMeshContainer(const xMeshContainer &Bn){ *this = Bn; }
	xMeshContainer(xMeshContainer &Bn){ *this = Bn; }
	xMeshContainer & operator= (const xMeshContainer & other){
		this->FileName = other.FileName;
		this->Geometry = other.Geometry;
		this->Skeleton = other.Skeleton;
		SkeletonAnimated = other.SkeletonAnimated;
		this->Animation = other.Animation;
		return *this;
	}
	xMeshContainer & operator= (xMeshContainer & other){
		this->FileName = other.FileName;
		this->Geometry = other.Geometry;
		this->Skeleton = other.Skeleton;
		SkeletonAnimated = other.SkeletonAnimated;
		this->Animation = other.Animation;
		return *this;
	}	

	std::string					FileName;

	std::vector<xMeshGeometry>	Geometry;

	xSkeleton					Skeleton;

	xSkeleton					SkeletonAnimated;

	xAnimationInfo				Animation;


};

}

#endif
