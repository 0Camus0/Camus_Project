#ifndef T1000_WINDOW_PROPERTIES_H
#define T1000_WINDOW_PROPERTIES_H

#include <vector>

namespace t1000 {
	class WindowParameters {
	public:
		enum {
			PORTRAIT_0 = 1
			, PORTRAIT_180 = 2
			, LANDSCAPE_0 = 4
			, LANDSCAPE_180 = 8
			, FULL_SCREEN = 16
			, RESERVED = 32
			, BORDERLESS = 64
			, RESIZEABLE = 128
			, SHOW_CURSOR = 256
			, MODIFIED_BY_DEVICE = 512
		};

		struct displaymode {
			short	Width;
			short	Height;
			int		RefreshRate;
		};

		struct adapter {
			wchar_t Desc[128];
			std::size_t DedicatedVideoMemory;
			std::size_t DedicatedSystemMemory;
			std::size_t SharedSystemMemory;
			std::vector<displaymode>	ResolutionList;
		};

		std::vector<adapter>	adapters;

		short	FullScreenSelectedWidth;
		short	FullScreenSelectedHeight;
		short	WindowedWidth;
		short	WindowedHeight;
		int		FullScreenSelectedRefreshRate;		
		int		Properties;

		WindowParameters() :			
			  FullScreenSelectedWidth(0)
			, FullScreenSelectedHeight(0)
			, WindowedWidth(1280)
			, WindowedHeight(720)
			, FullScreenSelectedRefreshRate(60)
			, Properties(LANDSCAPE_0 | RESIZEABLE | SHOW_CURSOR) {
			
			}

		~WindowParameters() {
		
		}

		//	To implement per platform
		void GatherProperties();

		void SetParametersFromDriver(short _Width, short _Height);

	};

	WindowParameters& GetWindowParameters();
}

#endif