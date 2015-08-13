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
#include "../osm/OsmVehicle.h"

// FORWARD REFERENCES
//

/** A simple data structure for holding the configuration
 *  of the vehicle we are routing.
 */
struct VehicleConfig
{
// ATTRIBUTES
	OsmVehicle::VehicleType	    category;
	double			            height;
	double			            length;
	double			            weight;
	double			            width;
	unsigned                    maxspeed;
	unsigned                    acceleration; // seconds 0 - 100 km/h
	unsigned                    deceleration; // seconds 100 - 0 km/h
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_VEHICLECONFIG_H_ */
