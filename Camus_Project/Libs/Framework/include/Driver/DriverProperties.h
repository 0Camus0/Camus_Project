#ifndef CA_DIVER_PROPERTIES_H
#define CA_DIVER_PROPERTIES_H

namespace hyperspace {

	class DriverProperties {
	public:

		enum {
			CHANNEL_RED = 1
			, CHANNEL_GREEN = 2
			, CHANNEL_BLUE = 4
			, CHANNEL_ALPHA = 8
		};

		enum {
			OPENGL_ES_1_0
			, OPENGL_ES_2_0
			, OPENGL_ES_3_0
			, OPENGL_DESKTOP_4_0
		};

		DriverProperties() :
			BitsPerDepth(16)
			, BitsPerPixel(8)
			, Channels(CHANNEL_RED | CHANNEL_GREEN | CHANNEL_BLUE | CHANNEL_ALPHA) {	}

		int		BitsPerDepth;
		int		BitsPerPixel;
		int		Channels;
	};

	DriverProperties& GetDriverProperties();

}

#endif