/**  Data structure for configuration of vehicle we are routing.
 *
 * #include "VehicleConfig.h"
 *
 * @author Jonas Bergman
 */
#ifndef CONFIG_VEHICLECONFIG_H_
#define CONFIG_VEHICLECONFIG_H_

// SYSTEM INCLUDES
//
#include <string>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//

// FORWARD REFERENCES
//

struct VehicleConfig
{
// ATTRIBUTES
	std::string		category;
	double			height;
	double			length;
	double			weight;
	double			width;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_VEHICLECONFIG_H_ */
