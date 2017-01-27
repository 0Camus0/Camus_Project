#include <Driver/Direct3D11Driver.h>
#include <Driver/DriverProperties.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>

void D3DReturnCodeCheck(char* f,HRESULT &r) {
	switch (r){
		case S_OK: {
			return;
		}break;
		case D3D11_ERROR_FILE_NOT_FOUND: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [D3D11_ERROR_FILE_NOT_FOUND]",f);
		}break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS]",f);
		}break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS]",f);
		}break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD]",f);
		}break;
		case DXGI_ERROR_INVALID_CALL: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [DXGI_ERROR_INVALID_CALL]",f);
		}break;
		case DXGI_ERROR_WAS_STILL_DRAWING: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [DXGI_ERROR_WAS_STILL_DRAWING]",f);
		}break;
		case E_FAIL: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [E_FAIL]",f);
		}break;
		case E_INVALIDARG: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [E_INVALIDARG]", f);
		}break;
		case E_OUTOFMEMORY: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [E_OUTOFMEMORY]",f);
		}break;
		case E_NOTIMPL: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [E_NOTIMPL]",f);
		}break;
		case S_FALSE: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [S_FALSE]",f);
		}break;
		default: {
			LogPrintError("[D3D Return Codes] Function [%s] Error [UNKNOWN]",f);
		}break;
	}
}	

namespace hyperspace {
	namespace video {
		Direct3D11Driver::Direct3D11Driver() {
			bInited				= false;
			DXGISwapchain		= 0;
			D3D11Device			= 0;
			D3D11DeviceContext	= 0;
		}

		Direct3D11Driver::~Direct3D11Driver() {

		}
		
		void	Direct3D11Driver::InitDriver() {
			auto &driver_properties = GetDriverProperties();
			auto &window_parameters = GetWindowParameters();

			int Width  = window_parameters.WindowedWidth;
			int Height = window_parameters.WindowedHeight;
			bool FS = (window_parameters.Properties & WindowParameters::FULL_SCREEN) ? true : false;
			
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
			
			HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 
				D3D11_CREATE_DEVICE_DEBUG
			, NULL, NULL,
				D3D11_SDK_VERSION, &SwapChainDesc, &DXGISwapchain, &D3D11Device, NULL, &D3D11DeviceContext);
#if USER_LOG_DEBUG_DRIVER_CALLS			
			D3DReturnCodeCheck("D3D11CreateDeviceAndSwapChain", hr);
#endif
			CreateSurfaces();

		}
		
		void	Direct3D11Driver::CreateSurfaces() {
			ID3D11Texture2D* BackBuffer;
			HRESULT hr = DXGISwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

#if USER_LOG_DEBUG_DRIVER_CALLS			
			D3DReturnCodeCheck("D3D11CreateDeviceAndSwapChain", hr);
#endif

			hr = D3D11Device->CreateRenderTargetView(BackBuffer, NULL, &D3D11RenderTargetView);
#if USER_LOG_DEBUG_DRIVER_CALLS			
			D3DReturnCodeCheck("D3D11CreateDeviceAndSwapChain", hr);
#endif
			BackBuffer->Release();

			D3D11DeviceContext->OMSetRenderTargets(1, &D3D11RenderTargetView, NULL);
		}
		
		void	Direct3D11Driver::DestroySurfaces() {
			D3D11DeviceContext->OMSetRenderTargets(1, 0, 0);
			D3D11RenderTargetView->Release();
			
		}
		
		void	Direct3D11Driver::Update() {
		
		}

		void	Direct3D11Driver::DestroyDriver() {
			DXGISwapchain->Release();
			D3D11Device->Release();
			D3D11DeviceContext->Release();
		}
		
		void	Direct3D11Driver::SetWindow(void *window) {
			hwnd = GetActiveWindow();
		}

		void	Direct3D11Driver::Clear(draw_bits_ mask, int r, int g, int b, int a, float depth) {
			float rgba[4];
			rgba[0] = static_cast<float>(r) / 255.0f;
			rgba[1] = static_cast<float>(g) / 255.0f;
			rgba[2] = static_cast<float>(b) / 255.0f;
			rgba[3] = static_cast<float>(a) / 255.0f;

			if (mask&draw_bits_::COLOR_BIT)
				D3D11DeviceContext->ClearRenderTargetView(D3D11RenderTargetView, rgba);	
				
		}

		void	Direct3D11Driver::Clear(draw_bits_ mask, float r, float g, float b, float a, float depth) {
			float rgba[4];
			rgba[0] = r;
			rgba[1] = g;
			rgba[2] = b;
			rgba[3] = a;

			if (mask&draw_bits_::COLOR_BIT)
				D3D11DeviceContext->ClearRenderTargetView(D3D11RenderTargetView, rgba);
		}
		
		void	Direct3D11Driver::SwapBuffers() {
			DXGISwapchain->Present(0, 0);
		}

		void	Direct3D11Driver::ResetDriver() {

		}
	}
}