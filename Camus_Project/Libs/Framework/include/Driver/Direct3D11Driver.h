#ifndef CA_D3D_DRIVER_H
#define CA_D3D_DRIVER_H

#include <Driver/BaseDriver.h>

#include <d3d11.h>

namespace hyperspace {
	namespace video {
		class Direc3D11Driver : public BaseDriver {
		public:
			Direc3D11Driver();
			~Direc3D11Driver();
			void	InitDriver();
			void	CreateSurfaces();
			void	DestroySurfaces();
			void	Update();

			void	DestroyDriver();
			void	SetWindow(void *window);


			void	Clear(draw_bits_ mask, int r, int g, int b, int a, float depth = 1.0f);
			void	Clear(draw_bits_ mask, float r, float g, float b, float a, float depth = 1.0f);
			void	SwapBuffers();

			void	ResetDriver();

			IDXGISwapChain		*DXGISwapchain;             
			ID3D11Device		*D3D11Device;                     
			ID3D11DeviceContext *D3D11DeviceContext;

			HWND				hwnd;

			bool				bInited;
		};
	}
}




#endif