#include <Config.h>
#ifndef OS_ANDROID


#ifndef T1000_TEXTURE_D3D_H
#define T1000_TEXTURE_D3D_H


#include <video\BaseDriver.h>


#include <d3d11.h>
#include <wrl.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace t800 {
  class D3DXTexture : public Texture {
  public:
    D3DXTexture() {}

    void	SetTextureParams();
    void	GetFormatBpp(unsigned int &props, unsigned int &Format, unsigned int &bpp);

    void	LoadAPITexture(DeviceContext* context, unsigned char* buffer);
    void	LoadAPITextureCompressed(unsigned char* buffer);
    void	DestroyAPITexture();
    void  Set(const DeviceContext& deviceContext, unsigned int slot, std::string shaderTextureName) override;
    void  SetSampler(const DeviceContext& deviceContext) override;

    ComPtr<ID3D11Texture2D>				Tex;
    ComPtr<ID3D11ShaderResourceView>    pSRVTex;
    ComPtr<ID3D11SamplerState>          pSampler;

  };
}
#endif

#endif // OS_ANDROID