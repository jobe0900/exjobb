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
	std::string		mCategory;
	double			mHeight;
	double			mLength;
	double			mWeight;
	double			mWidth;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_VEHICLECONFIG_H_ */
