/*=============================================================================
XDefs.h 	: .X Enums and Types
Revision history:
* Created by Camus
=============================================================================*/
#ifndef XDEFS_LEGACY_H
#define XDEFS_LEGACY_H

#include "../../Framework/Include/GlobalHeader.h"
#include "../../Framework/Include/FTMaths.h"
#include <string>
#include <iostream>

namespace stdEngine{

typedef unsigned long       stdDWORD;
typedef int                 stdBOOL;
typedef unsigned char       stdBYTE;
typedef unsigned short      stdWORD;
typedef float               stdFLOAT;
typedef std::string			stdSTRING;



typedef enum _STDEFFECTENUM
{
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

enum XTemplateType
{
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
	STD_X_REF,
};

const std::string XstringTemplates[] = 
{
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
};

#define MAXNUMTEMPLATES 22

struct xKeyValuePair
{
	std::string Key;
	std::string Value;
};

struct xMeshFace
{
	xMeshFace(){ NumVertexIndices = 0; Indices = STDNULL; }
	//~xMeshFace(){ delete [] Indices; }
	stdDWORD	NumVertexIndices;
	stdWORD*	Indices;
};

struct xTexCoords
{
	xTexCoords(){ NumTexcoords = 0; TexCoordinates = STDNULL;}
	//~xTexCoords(){ delete [] TexCoordinates;}
	stdDWORD		 NumTexcoords;
	STDVECTOR2		*TexCoordinates;
};

struct xGeometry
{
	xGeometry(){ NumVertices = NumFaces = 0; Vertices = STDNULL; Faces = STDNULL; }
	//~xGeometry(){ delete [] Vertices; delete [] Faces; }
	stdDWORD		 NumVertices;
	STDVECTOR3		*Vertices;
	stdDWORD		 NumFaces;
	xMeshFace		*Faces;
};

struct xVertexElement
{
	xVertexElement(){ Type = Method = Usage = UsageIndex = 0; }
	stdDWORD Type;
	stdDWORD Method;
	stdDWORD Usage;
	stdDWORD UsageIndex;
};

struct xDeclData
{
	xDeclData(){ NumElements = NumValues = 0; Elements = STDNULL; Values = STDNULL; }
	//~xDeclData(){ delete [] Elements; delete [] Values; }
	stdDWORD			 NumElements;
	xVertexElement		*Elements;
	stdDWORD			 NumValues;
	stdDWORD			*Values;
};

struct xSkinMeshHeader
{
	xSkinMeshHeader(){ MaxNumWeightPerVertex = MaxNumWeightPerFace = NumBones = NumBonesProcess = 0; }
	stdWORD	MaxNumWeightPerVertex;
	stdWORD	MaxNumWeightPerFace;
	stdWORD NumBones;
	stdWORD NumBonesProcess;
};

struct xSkinWeights
{
	xSkinWeights(){ NumWeights = 0; VertexIndices = STDNULL; Weights = STDNULL; }
	//~xSkinWeights() { delete [] VertexIndices; delete [] Weights; }
	std::string	  NodeName;
	stdDWORD	  NumWeights;
	stdDWORD	 *VertexIndices;
	stdFLOAT	 *Weights;
	STDMATRIX	  MatrixOffset;	
};

struct xSkinInfo
{
	xSkinInfo(){ SkinWeights = STDNULL; }
	//~xSkinInfo(){ delete [] SkinWeights; }
	xSkinMeshHeader	 SkinMeshHeader;
	xSkinWeights	*SkinWeights;
};

struct xFloatKeys
{
	xFloatKeys(){ NumValues = 0; Values = STDNULL; }
	stdDWORD	     NumValues;
	stdFLOAT	    *Values;
};

struct xTimedFloatKeys 
{ 
	xTimedFloatKeys(){ Time = 0; }
	stdDWORD		Time; 
	xFloatKeys		FloatKeys; 
};

struct xAnimationKey
{
	xAnimationKey(){ Type = NumKeys = STDNULL; Keys = STDNULL; }
	stdDWORD		  Type;
	stdDWORD		  NumKeys;
	xTimedFloatKeys  *Keys;
};

struct xAnimation
{
	std::string			Name;
	std::string			BoneName;
	xAnimationKey		Rot;
	xAnimationKey		Pos;
	xAnimationKey		Sca;
	xAnimationKey		Mtx;
};

struct xAnimationSet
{
	xAnimationSet(){ NumBones = 0; BoneAnimations = STDNULL; }
	std::string			Name;
	stdDWORD		NumBones;
	xAnimation	   *BoneAnimations;	
};

struct STDRGBAColor 
{
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

struct xEffectDefault
{
	xEffectDefault(){ NumElements = 0; CaseDWORD = 0;  CaseFloat = STDNULL; }
	//~xEffectDefault(){ delete [] CaseFloat;}
	std::string		NameParam;
	std::string		CaseString;
	STDEFFECTENUM	Type;
	stdDWORD		NumElements;
	stdDWORD		CaseDWORD;
	stdFLOAT*		CaseFloat;
	

};

struct xEffectInstance
{
	xEffectInstance(){ NumDefaults = 0; NumProcess = 0; pDefaults = STDNULL; }
	//~xEffectInstance(){ delete [] pDefaults; }
	std::string		 ShaderFileName;
	stdDWORD		 NumDefaults;
	stdDWORD		 NumProcess;
	xEffectDefault	*pDefaults;
};

struct xMaterial
{
	xMaterial(){ Power = 0.0f; bEffects = false; }
	//~xMaterial(){ }
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
	xMaterialList(){ NumMaterials = NumFaceIndices = 0; NumMatProcess = 0; FaceIndices = STDNULL; Materials = STDNULL; }
	//~xMaterialList(){ delete [] FaceIndices; }
	stdDWORD		 NumMaterials;
	stdDWORD		 NumFaceIndices;
	stdDWORD		 NumMatProcess;
	stdDWORD		*FaceIndices;
	xMaterial		*Materials;
};

typedef struct _xMESHDATA
{
	_xMESHDATA(){
		pNextMesh = STDNULL;
	}
	//~_xMESHDATA(){ pNextMesh = STDNULL; }

	std::string			Name;
	xGeometry			Vertex;
	xGeometry			Normals;
	xGeometry			Tangent;
	xGeometry			BiNormal;
	xTexCoords			TexCoords;
	xDeclData			DeclData;
	xSkinInfo			SkinInfo;
	xMaterialList		MaterialList;
	STDMATRIX			Relative;

	struct _xMESHDATA  *pNextMesh; 

} STDMESHDATA, *LPSTDMESHDATA;

typedef struct _xBONE
{
	_xBONE(){
		pBoneNextBrother = STDNULL;
		pBoneParent		 = STDNULL;
		pBoneFirstChild  = STDNULL;
		AsociatedMesh    = STDNULL; }

	std::string	    Name;
	STDMATRIX	    TransformationMatrix;
	STDMATRIX		CombinedTransformMatrix;
	STDMATRIX		ObjectCustomMatrix;
	LPSTDMESHDATA	AsociatedMesh;
	struct _xBONE	*pBoneNextBrother;
	struct _xBONE	*pBoneParent;
	struct _xBONE	*pBoneFirstChild;	

} STDBONE, *LPSTDBONE;

}

#endif
