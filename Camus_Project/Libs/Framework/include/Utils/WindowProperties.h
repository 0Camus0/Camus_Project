#ifndef CA_WINDOW_PROPERTIES_H
#define CA_WINDOW_PROPERTIES_H

#include <vector>

namespace hyperspace {
	class WindowParameters {
	public:
		enum {
			PORTRAIT_0 = 1
			, PORTRAIT_180 = 2
			, LANDSCAPE_0 = 4
			, LANDSCAPE_180 = 8
			, FULL_SCREEN = 16
			, WINDOWED = 32
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

		std::vector<displaymode>	ResolutionList;
		short	SelectedWidth;
		short	SelectedHeight;
		int		SelectedRefreshRate;		
		int		Properties;

		WindowParameters() :
			  SelectedWidth(1280)
			, SelectedHeight(720)
			, SelectedRefreshRate(60)
			, Properties(LANDSCAPE_0 | WINDOWED | RESIZEABLE | SHOW_CURSOR) {	
			
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