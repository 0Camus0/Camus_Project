#ifndef T1000_DIVER_PROPERTIES_H
#define T1000_DIVER_PROPERTIES_H

#include <string>
#include <vector>

namespace t1000 {
	namespace video {
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
				, Channels(CHANNEL_RED | CHANNEL_GREEN | CHANNEL_BLUE | CHANNEL_ALPHA) {	
			}

			~DriverProperties() {
			}

			bool isExtensionSupported(std::string str);

			void SetExtensions(std::string str);

			void ListExtensions();

			int			BitsPerDepth;
			int			BitsPerPixel;
			int			Channels;
			std::string	Version;
			std::string Extensions;
			std::vector<std::string>	Ext_tokenized;
		};

		DriverProperties& GetDriverProperties();
	}
}

#endif