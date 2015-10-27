/*=============================================================================
StaticMesh.h 	
Revision history:
* Created by Camus
=============================================================================*/

#include "../../XLoader/Include/XFileParser.h"
#include "TextureContainer.h"

using namespace stdEngine;

#ifndef STDSTATICMESH_H
#define STDSTATICMESH_H

enum{
	STDMESH_HAS_NORMALS		=   1,
	STDMESH_HAS_TANGENTS	=   2,
	STDMESH_HAS_BINORMALS	=   4,
	STDMESH_HAS_TC0			=   8,
	STDMESH_HAS_TC1			=   16
};

struct STD_SimpleVertex{
	STDVECTOR3	Pos;
	STDVECTOR2	TC;
	STDVECTOR3 Normal;
};

struct STD_TBNVertex{
	STDVECTOR3	Pos;
	STDVECTOR2	TC;
	STDVECTOR3 Normal;
	STDVECTOR3 Tangent;
	STDVECTOR3 Binormal;
};

struct STD_Vertex_Strides{
	STD_Vertex_Strides(){ PosStride = TC0Stride = TC1Stride = NormalStride = TangentStride = BinormalStride = -1;  }
	int	PosStride;
	int	TC0Stride;
	int TC1Stride;
	int NormalStride;
	int TangentStride;
	int BinormalStride;
};

struct MaterialInfo{
	MaterialInfo(){
		HasDiffuse = HasHalfLambert = HasNormal = HasSpecular = false;
		DiffuseId = NormalId = SpecularId = 0;
		SpecularPower = SpecularIntensity = 0.0f;
	}	
	~MaterialInfo(){}
	bool		HasDiffuse;
	bool		HasHalfLambert;
	bool		HasNormal;
	bool		HasSpecular;
	std_uint	DiffuseId;
	std_uint	NormalId;
	std_uint	SpecularId;
	stdEngine::stdFLOAT	SpecularPower;
	stdEngine::stdFLOAT	SpecularIntensity;
	STDVECTOR3	AmbientColor;
	STDVECTOR3	DiffuseColor;
	STDVECTOR3  SpecularColor;
};

struct STD_SubsetInfo{
	STD_SubsetInfo(){ Id = MeshBelong =VertexStart = NumVertex = TriStart = NumTris = 0; }
	~STD_SubsetInfo(){}
	std_uint		Id;
	std_uint		MeshBelong;
	std_uint		VertexStart;
	std_uint		NumVertex;
	std_uint		TriStart;
	std_uint		NumTris;
	STDMATRIX		WorldMatrix;
	MaterialInfo	Material;
};

class STD_Static_Mesh{
public:
	STD_Static_Mesh();
	bool					LoadMesh(string Path);
	STD_SubsetInfo*			GetSubsetInfo(std_uint index);
	STD_Vertex_Strides*		GetStrides();
	inline std_uint			GetNumSubsets(){ return NumSubsets; };
	inline std_uint			GetNumTris(){ return NumTris; };
	inline std_uint			GetNumVertices(){ return NumVertices; };
	inline std_uint			GetNumBytesPerVertex(){ return BytesPerVertex; };
	inline std_uint			GetHas(){ return HasThings; };

	inline std_uint			GetIBO(){ return IndexBuffObject; }
	inline std_uint			GetVBO(){ return VertexBuffObject; }

	void				Release();
//private:
	STD_SubsetInfo		*Subsets;
	STD_SubsetInfo		*Meshes;

	std_uint			NumMeshes;
	std_uint			NumSubsets;
	std_uint			NumTris;
	std_uint			NumVertices;
	std_uint			BytesPerVertex;
	std_uint			HasThings;

	std_uint			IndexBuffObject;
	std_uint			VertexBuffObject;

	

protected:
	Parser	FileParser;
	void	GetEffectInstance(MaterialInfo* mat,stdEngine::xEffectInstance* effect);
};


#endif