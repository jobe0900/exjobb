/**  The Restrictions class contains different restrictions in the graph
 * such as dimensions, access, turn restrictions.
 *
 * #include "Restrictions.h" <BR>
 *
 * @author  Jonas Bergman
 */

#ifndef GRAPH_RESTRICTIONS_H_
#define GRAPH_RESTRICTIONS_H_

// SYSTEM INCLUDES
//
#include <limits>
#include <map>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "../osm/OsmAccess.h"
#include "../osm/OsmBarrier.h"
#include "../osm/OsmVehicle.h"

// FORWARD REFERENCES
//

/** Maps EdgeId => restriction.
 * Restrictions maps are:
 * - vehicle properties
 * - General access
 * - vehicle type specific access
 * - barriers
 * - turn restrictions
 * - disused roads
 * - no-exit roads
 */
class Restrictions
{
public:
// TYPES
    /** Restrictions on Vehicles to travel an Edge.
     *  Dimensions in meters.
     *  Speed in km/h
     */
    struct VehicleProperties
    {
        static double DEFAULT_DIMENSION_MAX {std::numeric_limits<double>::max()};
        static double DEFAULT_SPEED_MAX {std::numeric_limits<unsigned>::max()};
        static double DEFAULT_SPEED_MIN {0};

        double      maxHeight   {DEFAULT_DIMENSION_MAX};
        double      maxLength   {DEFAULT_DIMENSION_MAX};
        double      maxWeight   {DEFAULT_DIMENSION_MAX};
        double      maxWidth    {DEFAULT_DIMENSION_MAX};

        unsigned    maxSpeed    {DEFAULT_SPEED_MAX};
        unsigned    minSpeed    {DEFAULT_SPEED_MIN};
    };

// LIFECYCLE

    /** Default constructor.
     */
    Restrictions();


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    Restrictions(const Restrictions& from) = default;


    /** Destructor.
     */
    ~Restrictions() = default;


// OPERATORS
// OPERATIONS
// ACCESS
// INQUIRY

protected:
private:
    std::map<EdgeIdType, VehicleProperties>         mVehiclePropertiesMap;
    std::map<EdgeIdType, OsmAccess>                 mGeneralAccessMap;
    std::map<EdgeIdType, std::map<OsmVehicle::VehicleType, OsmAccess> >
                                                    mVehicleTypeAccess;
    std::map<EdgeIdType, OsmBarrier>                mBarrier;
// TODO  OsmTurningRestriction                              mTurnRestrictions;
    std::map<EdgeIdType, bool>                      mDisused;
    std::map<EdgeIdType, bool>                      mNoExit;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//



#endif /* GRAPH_RESTRICTIONS_H_ */
