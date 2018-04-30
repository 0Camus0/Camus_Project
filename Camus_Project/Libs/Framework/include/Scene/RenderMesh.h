#ifndef T1000_MESH_D3D_H
#define T1000_MESH_D3D_H

#include <Config.h>

#ifdef USING_GL_COMMON
#include <video/gl/GLDriver.h>
#include <video/gl/GLTexture.h>
#endif

#if defined(OS_WINDOWS)
#include <video/d3d11/D3DXTexture.h>
#include <D3Dcompiler.h>
#endif


#include <video/BaseDriver.h>

#include <utils/Utils.h>

#include <utils/xMaths.h>
#include <utils/XDataBase.h>
#include <scene/PrimitiveBase.h>



#include <vector>
#include <memory>
using namespace xF;
namespace t1000 {
  class RenderMesh : public PrimitiveBase {
  public:
    RenderMesh() {
      d3dxEnvMap = 0;
      EnvMap = 0;
    }

    struct CBuffer {
      XMATRIX44 WVP;
      XMATRIX44 World;
      XMATRIX44 WorldView;
      XVECTOR3  Light0Pos;
      XVECTOR3  Light0Col;
      XVECTOR3  CameraPos;
      XVECTOR3  CameraInfo;
      XVECTOR3  AmbientColor;
	  XVECTOR3  DiffuseColor;
	  XVECTOR3  SpecularColor;
	  XVECTOR3  FresnelColor;
	  XVECTOR3  Intensities;
	  XVECTOR3  ParallaxSettings;
    };


    struct SubSetInfo {
		SubSetInfo() {
			AmbientColor = XVECTOR3(0.0f, 0.0f, 0.0f, 1.0f);
			DiffuseColor = XVECTOR3(0.5f, 0.5f, 0.5f, 1.0f);
			SpecularColor = XVECTOR3(1.0f, 1.0f, 1.0f, 1.0f);
			FresnelColor = XVECTOR3(1.0f, 1.0f, 1.0f, 1.0f);
			Intensities = XVECTOR3(1.0f, 8.0f, 1.0f, 1.0f);
			bUseFresnel = false;
			MatID = 0;
		}
      unsigned long long		Sig;

      t1000::IndexBuffer*  	IB;
      Texture*					DiffuseTex;
      Texture*					SpecularTex;
      Texture*					GlossfTex;
      Texture*					NormalTex;
      Texture*					ReflectTex;
      Texture*					ParalaxTex;

	  XVECTOR3		  AmbientColor;
	  XVECTOR3	      DiffuseColor;
	  XVECTOR3		  SpecularColor;
	  XVECTOR3		  FresnelColor;
	  XVECTOR3        Intensities;

      int					DiffuseId;
      int					SpecularId;
      int					GlossfId;
      int					NormalId;
      int					ReflectId;
      int					ParalaxId;
	  
	  int					MatID;

      unsigned int		VertexStart;
      unsigned int		NumVertex;
      unsigned int		TriStart;
      unsigned int		NumTris;
      unsigned int		VertexSize;
      bool				bAlignedVertex;
	  bool				bUseFresnel;
    };


    struct MeshInfo {
      unsigned int			 VertexSize;
      unsigned int			 NumVertex;

	  t1000::IndexBuffer*  	IB;
	  t1000::VertexBuffer*  	VB;
	  t1000::ConstantBuffer* CB;
      RenderMesh::CBuffer			CnstBuffer;

      std::vector<SubSetInfo>	SubSets;
    };

    void Load(char *);
    void Create();
    void Transform(float *t);
    void Draw(float *t, float *vp);
    void Destroy();

    void GatherInfo();
    int  LoadTex(std::string p, xF::xMaterial *mat, Texture** tex);

    Texture*	d3dxEnvMap;

    XMATRIX44	transform;
    XDataBase*	xFile;
    std::vector<MeshInfo> Info;
  };
}

#endif


