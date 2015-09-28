#ifndef CA_WINDOW_PROPERTIES_H
#define CA_WINDOW_PROPERTIES_H

class WindowParameters {
	public:
		enum {
			 PORTRAIT_0			= 1
			,PORTRAIT_180		= 2 
			,LANDSCAPE_0		= 4
			,LANDSCAPE_180		= 8
			,FULL_SCREEN		= 16
			,WINDOWED			= 32
			,BORDERLESS			= 64
			,RESIZEABLE			= 128
			,SHOW_CURSOR		= 256
			,MODIFIED_BY_DEVICE = 512
		};

		short	Width;
		short	Height;
		int		Properties;

		WindowParameters() :
		Width(1280)
		,Height(720)
		,Properties(LANDSCAPE_0|WINDOWED|RESIZEABLE| SHOW_CURSOR) {	}

		//	To implement per platform
		void GatherProperties();
		
		void SetParametersFromDriver(short _Width,short _Height);

};

WindowParameters& GetWindowParameters();


#endif