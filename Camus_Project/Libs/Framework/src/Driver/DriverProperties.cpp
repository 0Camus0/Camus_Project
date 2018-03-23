#include <Driver\DriverProperties.h>
#include <Utils\Log.h>

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

namespace t1000 {
	namespace video {
		DriverProperties& GetDriverProperties() {
			static DriverProperties	_driverproperties;
			return _driverproperties;
		}

		bool DriverProperties::isExtensionSupported(std::string str) {
			return (Extensions.find(str) != std::string::npos);
		}

		void DriverProperties::SetExtensions(std::string str) {
			Extensions = str;
			std::istringstream iss(str);
			std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>{} };

			Ext_tokenized = tokens;
		}

		void DriverProperties::ListExtensions() {
			for (unsigned int i = 0; i < Ext_tokenized.size(); i++) {
				LogPrintDebug("[%s]", Ext_tokenized[i].c_str());
			}
		}
	}
}