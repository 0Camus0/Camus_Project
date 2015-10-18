#include <Driver/DriverProperties.h>
namespace hyperspace {
	namespace video {
		DriverProperties& GetDriverProperties() {
			static DriverProperties	_driverproperties;
			return _driverproperties;
		}
	}
}