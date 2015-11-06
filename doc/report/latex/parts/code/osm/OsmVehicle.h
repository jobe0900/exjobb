/**  Access to Vehicle data from the OSM file.
 *
 * #include "OsmVehicle.h"
 *
 * @author  Jonas Bergman
 */

#ifndef OSM_OSMVEHICLE_H_
#define OSM_OSMVEHICLE_H_

// SYSTEM INCLUDES
//
#include <string>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "OsmException.h"

// FORWARD REFERENCES
//

/**
 * Class for working with different categories of vehicles.
 */
class OsmVehicle
{
public:
// TYPES
    enum VehicleType
    {
        MOTORCAR,
        GOODS,
        HGV,
        PSV,
        LHV,
        MOTOR_VEHICLE,
        VEHICLE,

        NR_VEHICLE_TYPES
    };

// LIFECYCLE
    OsmVehicle() = delete;
    OsmVehicle(VehicleType type);
    OsmVehicle(const OsmVehicle& from) = default;
    ~OsmVehicle() = default;

// OPERATORS
// OPERATIONS
    /** Attempt to parse a string to a VehicleType
     * @param   rTypeString     String which could contain a Vehicle type
     * @return  A valid VehicleType
     * @throw   OsmException if invalid string.
     */
    static VehicleType  parseString(const std::string& rTypeString);

    /** Convert a Vehicle Type to a string representation.
     * @param   vehicleType     The type to convert.
     * @return  string representation of the type.
     * @throw   OsmException if unknown vehicle type (out of bounds).
     */
    static std::string  toString(VehicleType vehicleType);

    /** Convert this VehicleType to a string.
     * @return  string representation of this VehicleType.
     */
    std::string         toString() const;

// ACCESS
// INQUIRY
protected:
private:
    VehicleType              mType {VEHICLE};
    static const std::string sTypeStrings[];
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_OSMVEHICLE_H_ */
