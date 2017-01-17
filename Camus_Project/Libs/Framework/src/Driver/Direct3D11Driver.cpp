#include <Driver/Direct3D11Driver.h>
#include <Driver/DriverProperties.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>

namespace hyperspace {
	namespace video {
		Direc3D11Driver::Direc3D11Driver() {
			bInited				= false;
			DXGISwapchain		= 0;
			D3D11Device			= 0;
			D3D11DeviceContext	= 0;
		}

		Direc3D11Driver::~Direc3D11Driver() {

		}
		
		void	Direc3D11Driver::InitDriver() {
			auto &driver_properties = GetDriverProperties();
			auto &window_parameters = GetWindowParameters();

			int Width  = window_parameters.WindowedWidth;
			int Height = window_parameters.WindowedHeight;
			bool FS = window_parameters.Properties & WindowParameters::FULL_SCREEN;
			
			if (FS) {
				int Width = window_parameters.FullScreenSelectedWidth;
				int Height = window_parameters.FullScreenSelectedHeight;
			}

			DXGI_MODE_DESC BackBufferDesc;
			ZeroMemory(&BackBufferDesc, sizeof(DXGI_MODE_DESC));
			BackBufferDesc.Width  = Width;
			BackBufferDesc.Height = Height;
			BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			BackBufferDesc.RefreshRate.Numerator = window_parameters.FullScreenSelectedRefreshRate;
			BackBufferDesc.RefreshRate.Denominator = 1;
			BackBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			BackBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			DXGI_SWAP_CHAIN_DESC SwapChainDesc;
			ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			SwapChainDesc.BufferDesc = BackBufferDesc;
			SwapChainDesc.BufferCount = 1;
			SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.SampleDesc.Count = 1;
			SwapChainDesc.SampleDesc.Quality = 0;
			SwapChainDesc.OutputWindow = hwnd;
			SwapChainDesc.Windowed = !FS;
			SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			
			
		}
		
		void	Direc3D11Driver::CreateSurfaces() {
		
		}
		
		void	Direc3D11Driver::DestroySurfaces() {
		
		}
		
		void	Direc3D11Driver::Update() {
		
		}

		void	Direc3D11Driver::DestroyDriver() {
		
		}
		
		void	Direc3D11Driver::SetWindow(void *window) {
			hwnd = GetActiveWindow();
		}

		void	Direc3D11Driver::Clear(draw_bits_ mask, int r, int g, int b, int a, float depth) {

		}

		void	Direc3D11Driver::Clear(draw_bits_ mask, float r, float g, float b, float a, float depth) {
		
		}
		
		void	Direc3D11Driver::SwapBuffers() {

		}

		void	Direc3D11Driver::ResetDriver() {

		}
	}
}