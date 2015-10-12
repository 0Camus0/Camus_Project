#include <Driver/DriverProperties.h>
namespace hyperspace {
	DriverProperties& GetDriverProperties() {
		static DriverProperties	_driverproperties;
		return _driverproperties;
	}
}