#include <Driver/DriverProperties.h>

DriverProperties& GetDriverProperties() {
	static DriverProperties	_driverproperties;
	return _driverproperties;
}