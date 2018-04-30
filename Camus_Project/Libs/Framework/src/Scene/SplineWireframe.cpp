#include <scene/SplineWireframe.h>
#include <utils/Utils.h>

namespace t1000 {
  extern Device*            T8Device;
  extern DeviceContext*     T8DeviceContext;
void SplineWireframe::Create()
{
  char *vsSourceP;
  char *fsSourceP;
  if (g_pBaseDriver->m_currentAPI == T_GRAPHICS_API::_OPENGL) {
    vsSourceP = file2string("VS_W.glsl");
    fsSourceP = file2string("FS_W.glsl");
  }
  else {
    vsSourceP = file2string("VS_W.hlsl");
    fsSourceP = file2string("FS_W.hlsl");
  }


  std::string vstr = std::string(vsSourceP);
  std::string fstr = std::string(fsSourceP);

#ifdef USING_OPENGL
  std::string Defines = "";
  Defines += "#version 130\n\n";
  Defines += "#define lowp \n\n";
  Defines += "#define mediump \n\n";
  Defines += "#define highp \n\n";

  vstr = Defines + vstr;
  fstr = Defines + fstr;
#endif

  free(vsSourceP);
  free(fsSourceP);

  shaderID = g_pBaseDriver->CreateShader(vstr, fstr, T_NO_SIGNATURE);
  s = g_pBaseDriver->GetShaderIdx(shaderID);


  for (float i = 0; i < (float)m_spline->m_points.size() - 3.0f; i+= m_spline->STEP_SIZE) {
    XVECTOR3 v = m_spline->GetPoint(i);
    vertices.push_back(Vert{ v.x,v.y,v.z,1.0f });
  }
  for (std::size_t i = 0; i < vertices.size(); i++) {
    indices.push_back((unsigned short)i);
  }

  t1000::BufferDesc bdesc;
  bdesc.byteWidth = sizeof(CBuffer);
  bdesc.usage = T_BUFFER_USAGE::DEFAULT;
  CB = (t1000::ConstantBuffer*)T8Device->CreateBuffer(T_BUFFER_TYPE::CONSTANT, bdesc);

  bdesc.byteWidth = sizeof(Vert) * vertices.size();
  bdesc.usage = T_BUFFER_USAGE::DEFAULT;
  VB = (t1000::VertexBuffer*)T8Device->CreateBuffer(T_BUFFER_TYPE::VERTEX, bdesc, &vertices[0]);


  bdesc.byteWidth = indices.size() * sizeof(unsigned short);
  bdesc.usage = T_BUFFER_USAGE::DEFAULT;
  IB = (t1000::IndexBuffer*)T8Device->CreateBuffer(T_BUFFER_TYPE::INDEX, bdesc, &indices[0]);
  
}
void SplineWireframe::Transform(float * t)
{
}
void SplineWireframe::Draw(float * t, float * vp)
{

  Camera *pActualCamera = pScProp->pCameras[0];
  constantBuff.WVP = pActualCamera->VP;
  IB->Set(*T8DeviceContext, 0, T_IB_FORMAT::R16);
  VB->Set(*T8DeviceContext,sizeof(Vert),0);
  s->Set(*T8DeviceContext);
  CB->UpdateFromBuffer(*T8DeviceContext, &constantBuff.WVP[0]);
  CB->Set(*T8DeviceContext);
  T8DeviceContext->SetPrimitiveTopology(T_TOPOLOGY::LINE_STRIP);
  T8DeviceContext->DrawIndexed(vertices.size(), 0, 0);
}
void SplineWireframe::Destroy()
{
}
}