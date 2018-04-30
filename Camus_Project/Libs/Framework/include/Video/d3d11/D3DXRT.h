#include <Config.h>
#ifndef OS_ANDROID


#ifndef T1000_D3DXRT_H
#define T1000_D3DXRT_H

#include <video/BaseDriver.h>
#include <video/d3d11/D3DXTexture.h>

#include <d3d11.h>
#include <dxgi.h>
#include <vector>

#include <wrl.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace t800 {
  class D3DXRT : public BaseRT {
  public:
    bool			LoadAPIRT();
    void			DestroyAPIRT();
    void Set(const DeviceContext& context) override;
    void ChangeCubeDepthTexture(int i) override;
    std::vector<ComPtr<ID3D11RenderTargetView>>		vD3D11RenderTargetView;
    std::vector<ComPtr<ID3D11Texture2D>>			vD3D11ColorTex;
    ComPtr<ID3D11Texture2D>							D3D11DepthTex;
    ComPtr<ID3D11DepthStencilView>					D3D11DepthStencilTargetView;
  };
}


#endif

#endif OS_ANDROID