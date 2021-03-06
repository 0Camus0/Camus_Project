
/*=============================================================================
xOGLESMesh.h  : Mesh implementation for OGLES2 - VBO/IBO
Revision history:
* Created by Camus
=============================================================================*/

#ifndef XOGLMESH_H
#define XOGLMESH_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "XDefs.h"

#include "../../Framework/Include/Timer.h"


using namespace xF;

struct xSubsetInfo{
	xSubsetInfo():VertexAttrib(0),Id(0),VertexStart(0),NumVertex(0),TriStart(0),NumTris(0),VertexSize(0){}
	~xSubsetInfo(){}
	unsigned long		VertexAttrib;
	unsigned int		Id;
	unsigned int		VertexStart;
	unsigned int		NumVertex;
	unsigned int		TriStart;
	unsigned int		NumTris;
	unsigned int		VertexSize;
	bool				bAlignedVertex;
};

struct xFinalGeometry{

	xFinalGeometry():Id(0),IdIBO(0),VertexSize(0),NumVertex(0),pData(0){}


	xFinalGeometry(const xFinalGeometry &fg){ *this = fg; }
	xFinalGeometry(xFinalGeometry &fg){ *this = fg; }
	xFinalGeometry & operator= (const xFinalGeometry & other){
		this->Id = other.Id;
		this->IdIBO = other.IdIBO;
		this->VertexSize = other.VertexSize;
		this->NumVertex = other.NumVertex;

		unsigned int NumFloatsPerVertex = this->VertexSize/4;
		unsigned int NumFloatsPerGeometry = NumFloatsPerVertex*this->NumVertex;

		pData = new float[NumFloatsPerGeometry];
		pDataDest = new float[NumFloatsPerGeometry];
		for (int i = 0; i < NumFloatsPerGeometry ; i++){
			pData[i] = other.pData[i];
			pDataDest[i] = other.pDataDest[i];
		}

		for (int i = 0; i < other.Subsets.size() ; i++){
			this->Subsets.push_back(other.Subsets[i]);
		}

		return *this;
	}
	xFinalGeometry & operator= (xFinalGeometry & other){
		this->Id = other.Id;
		this->IdIBO = other.IdIBO;
		this->VertexSize = other.VertexSize;
		this->NumVertex = other.NumVertex;

		unsigned int NumFloatsPerVertex = this->VertexSize/4;
		unsigned int NumFloatsPerGeometry = NumFloatsPerVertex*this->NumVertex;

		pData = new float[NumFloatsPerGeometry];
		pDataDest = new float[NumFloatsPerGeometry];
		for (int i = 0; i < NumFloatsPerGeometry ; i++){
			pData[i] = other.pData[i];
			pDataDest[i] = other.pDataDest[i];
		}

		for (int i = 0; i < other.Subsets.size() ; i++){
			this->Subsets.push_back(other.Subsets[i]);
		}

		return *this;
	}
	~xFinalGeometry(){ 	
		delete [] pData;
		delete [] pDataDest;
		pData = 0;
	}

	float					*pData;
	float					*pDataDest;
	std::vector<xSubsetInfo> Subsets;
	unsigned int			 Id;
	unsigned int			 IdIBO;
	unsigned int			 VertexSize;
	unsigned int			 NumVertex;
};

struct xMeshDesc{
	xMeshDesc():bAlignMemory(true),bHardwareSkinning(false){}
	bool bAlignMemory;
	bool bHardwareSkinning;
};

class AnimationController{
public:
	enum{
		POSITION_KEY = 1,
		ROTATION_KEY = 2,
		SCALE_KEY = 4,
	};
	AnimationController(xF::xAnimationInfo* aInfo,xF::xSkeleton *skInfoBind,xF::xSkeleton *skInfoOut);

	void	Update();
	

	void	NextAnimationSet();
	void	PrevAnimationSet();
	void	ResetAnimationSet();
	void    ToogleLoop();
	void	ToogleLerp();

	void	IncreaseSpeed();
	void	ReduceeSpeed();

private:

	void	ResetLocals(int bitflag);
	void	UpdateNoLerp();
	void	UpdateLerp();

	xF::xAnimationInfo  *m_pAInfo;
	xF::xSkeleton		*m_pSkeletonBind;
	xF::xSkeleton		*m_pSkeletonOut;
	Timer				 m_Timer;
	int					 m_ActualAnimationSet;
	unsigned	int		 m_TotalAnimationsSets;
	bool				 m_LoopingAnimations;
	bool				 m_LerpAnimations;
	float				 m_Speed;

	long			Ticks;
	double			msPerTick;
	
};

class xMesh : public xMeshContainer {
	public:
		xMesh(){}
		xMesh(const xMesh &Bn){ *this = Bn; }
		xMesh(xMesh &Bn){ *this = Bn; }
		xMesh & operator= (const xMesh & other){
			for (int i = 0; i < other.MeshInfo.size() ; i++){
				this->MeshInfo.push_back(other.MeshInfo[i]);
			}
			for (int i = 0; i < other.Geometry.size(); i++){
				this->Geometry.push_back(other.Geometry[i]);
			}
			this->Desc = other.Desc;
			this->FileName = other.FileName;
			this->Geometry = other.Geometry;
			//this->Skeleton = other.Skeleton;
			for (int i = 0; i < other.Skeleton.NumBones; i++)
			{
				this->Skeleton.Bones.push_back(other.Skeleton.Bones[i]);
				this->SkeletonAnimated.Bones.push_back(other.SkeletonAnimated.Bones[i]);
			}
			this->Animation = other.Animation;
			return *this;
		}
		xMesh & operator= (xMesh & other){
			for (int i = 0; i < other.MeshInfo.size() ; i++){
				this->MeshInfo.push_back(other.MeshInfo[i]);
			}
			for (int i = 0; i < other.Geometry.size(); i++){
				this->Geometry.push_back(other.Geometry[i]);
			}
			this->Desc = other.Desc;
			this->FileName = other.FileName;
			this->Geometry = other.Geometry;
			//this->Skeleton = other.Skeleton;
			for (int i = 0; i < other.Skeleton.NumBones; i++)
			{
				this->Skeleton.Bones.push_back(other.Skeleton.Bones[i]);
				this->SkeletonAnimated.Bones.push_back(other.SkeletonAnimated.Bones[i]);
			}
			this->Animation = other.Animation;
			return *this;
		}	
		~xMesh(){
			MeshInfo.clear();
			Geometry.clear();
		}

	void	CreateBuffers();
	void	ReleaseBuffers();
	
	void	Update();

	void	UpdateSkeleton(xBone *pBone, XMATRIX44 mat);

	void	UpdateSoftwareSkinning();

	void	UpdateAnimation();

	

	std::vector<xFinalGeometry>	 MeshInfo;
	AnimationController			 *m_AnimController;

	private:

	void	BuildSubsets();


	
	xMeshDesc					 Desc;
	

	
};


#endif