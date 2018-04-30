#include <Config.h>

#ifndef T1000_TEXTURE_GL_H
#define T1000_TEXTURE_GL_H

#include <video/BaseDriver.h>

namespace t1000 {
  class GLTexture : public Texture {
  public:
    GLTexture();
    virtual ~GLTexture() {}
    void	SetTextureParams();
    void	GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp);

    void	LoadAPITexture(DeviceContext* context, unsigned char* buffer);
    void	LoadAPITextureCompressed(unsigned char* buffer);
    void	DestroyAPITexture();

    void  Set(const DeviceContext& deviceContext, unsigned int slot, std::string shaderTextureName) override;
    void  SetSampler(const DeviceContext& deviceContext) override;

    unsigned int glTarget;
  private:
    int APITextureLoc;
  };
}

#endif
