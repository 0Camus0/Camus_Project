#include <Utils/WindowProperties.h>
#include <Utils/Log.h>

#if defined(OS_WIN32)
#include <dxgi.h>
#include <iostream>
#endif

namespace hyperspace {
	WindowParameters& GetWindowParameters() {
		static WindowParameters	_windowparameter;
		return _windowparameter;
	}

	void	WindowParameters::GatherProperties() {
#if defined(OS_WIN32)
		IDXGIFactory * pFactory = NULL;

		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);	

		IDXGIAdapter *pAdapter = 0;
		for (UINT i = 0;pFactory->EnumAdapters(i, &pAdapter)!= DXGI_ERROR_NOT_FOUND;++i){
			DXGI_ADAPTER_DESC adapter_desc;
			pAdapter->GetDesc(&adapter_desc);

#if USE_LOG_WINDOW_PROPERTIES
			char desc_str[256];
			sprintf_s(desc_str, "%ws", adapter_desc.Description);
			LogPrintDebug("[GetWindowParameters] Adapter [%d] Name [%s]", i, desc_str);
#endif		
			adapter tmp_adapter;
			wcsncpy_s(tmp_adapter.Desc, adapter_desc.Description, 128);
			tmp_adapter.DedicatedSystemMemory = adapter_desc.DedicatedSystemMemory;
			tmp_adapter.DedicatedVideoMemory = adapter_desc.DedicatedVideoMemory;
			tmp_adapter.SharedSystemMemory = adapter_desc.SharedSystemMemory;

#if USE_LOG_WINDOW_PROPERTIES
			LogPrintDebug("[GetWindowParameters] Adapter [%d] DedicatedSystemMemory [%d] MB", i, tmp_adapter.DedicatedSystemMemory / 1048576);
			LogPrintDebug("[GetWindowParameters] Adapter [%d] DedicatedVideoMemory [%d] MB", i, tmp_adapter.DedicatedVideoMemory / 1048576);
			LogPrintDebug("[GetWindowParameters] Adapter [%d] SharedSystemMemory [%d] MB", i, tmp_adapter.SharedSystemMemory / 1048576);
#endif	

			HRESULT hr;
			IDXGIOutput* pOutput = NULL;
			hr = pAdapter->EnumOutputs(0, &pOutput);

			if (hr == DXGI_ERROR_NOT_FOUND) {
#if USE_LOG_WINDOW_PROPERTIES
				LogPrintDebug("[GetWindowParameters] Not enum outputs Available");
#endif	
				adapters.push_back(tmp_adapter);
				continue;
			}

			UINT numModes = 0;
			DXGI_MODE_DESC* displayModes = NULL;
			DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
			
			pOutput->GetDisplayModeList(format, 0, &numModes, NULL);

			displayModes = new DXGI_MODE_DESC[numModes];

			pOutput->GetDisplayModeList(format, 0, &numModes, displayModes);

			for (unsigned int j = 0; j < numModes; j++) {
				displaymode dp;
				dp.Width = displayModes[j].Width;
				dp.Height = displayModes[j].Height;
				dp.RefreshRate = displayModes[j].RefreshRate.Numerator / displayModes[j].RefreshRate.Denominator;
				tmp_adapter.ResolutionList.push_back(dp);				
#if USE_LOG_WINDOW_PROPERTIES
				LogPrintDebug("[GetWindowParameters] Adapter [%d] Mode [%d] Resolution[%d x %d @ %d]", i, j, displayModes[j].Width, displayModes[j].Height, displayModes[j].RefreshRate.Numerator / displayModes[j].RefreshRate.Denominator);
#endif
			}

			adapters.push_back(tmp_adapter);

			delete [] displayModes;
		}


		adapter* basic_adapter = &adapters[0];


		displaymode *dp = &basic_adapter->ResolutionList.back();
		FullScreenSelectedWidth = dp->Width;
		FullScreenSelectedHeight = dp->Height;
		FullScreenSelectedRefreshRate = dp->RefreshRate;
#if USE_LOG_WINDOW_PROPERTIES
		LogPrintDebug("[GetWindowParameters] Full Screen Resolution selected [%d x %d @ %d]", FullScreenSelectedWidth, FullScreenSelectedHeight, FullScreenSelectedRefreshRate);
#endif
		

		
#elif defined(OS_ANDROID)

#endif
	}

	void WindowParameters::SetParametersFromDriver(short _Width, short _Height) {
		if (WindowedWidth != _Width || WindowedHeight != _Height)
			Properties |= MODIFIED_BY_DEVICE;
		
		WindowedWidth = _Width;
		WindowedHeight = _Height;
		
		LogPrintDebug("SetParametersFromDriver %d  %d", WindowedWidth, WindowedHeight);
	}
}