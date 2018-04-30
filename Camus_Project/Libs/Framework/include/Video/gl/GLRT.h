#include <Config.h>

#ifndef T1000_GLESXRT_H
#define T1000_GLESXRT_H

#include <video/BaseDriver.h>
#include <video/gl/GL_Headers.h>
#include <vector>

namespace t1000 {
  class GLRT : public BaseRT {
  public:

    virtual ~GLRT() {}
    bool	LoadAPIRT() override;
    void	DestroyAPIRT() override;
    void Set(const DeviceContext& context) override;
    void ChangeCubeDepthTexture(int i);
    std::vector<GLuint>		vFrameBuffers;
    std::vector<GLuint>		vGLColorTex;
    GLuint					DepthTexture;
  };
}


#endif
