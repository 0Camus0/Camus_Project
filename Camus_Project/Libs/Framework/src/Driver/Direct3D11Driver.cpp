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
			DXGI_SWAP_CHAIN_DESC SwapChainDesc;
			SwapChainDesc.BufferCount = 1;
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc.
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