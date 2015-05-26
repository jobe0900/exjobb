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
#include "RestrictionsException.h"
#include "../osm/OsmAccess.h"
#include "../osm/OsmBarrier.h"
#include "../osm/OsmTurningRestriction.h"
#include "../osm/OsmVehicle.h"
#include "Edge.h"
#include "Vertex.h"

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
        static constexpr double
            DEFAULT_DIMENSION_MAX {std::numeric_limits<double>::max()};
        static constexpr unsigned
            DEFAULT_SPEED_MAX {std::numeric_limits<unsigned>::max()};
        static constexpr unsigned
            DEFAULT_SPEED_MIN {0};

        double      maxHeight   {DEFAULT_DIMENSION_MAX};
        double      maxLength   {DEFAULT_DIMENSION_MAX};
        double      maxWeight   {DEFAULT_DIMENSION_MAX};
        double      maxWidth    {DEFAULT_DIMENSION_MAX};

        unsigned    maxSpeed    {DEFAULT_SPEED_MAX};
        unsigned    minSpeed    {DEFAULT_SPEED_MIN};
    };

    enum RestrictionType
    {
        VEHICLE_PROPERTIES,
        GENERAL_ACCESS,
        VEHICLE_TYPE_ACCESS,
        BARRIER,
        TURNING,
        DISUSED,
        NO_EXIT,

        NR_RESTRICTION_TYPES
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
    /** Set vehicle properties for the specified edge.
     * Replacing any existing properties with the new ones.
     * @param   edgeId              Id of the edge.
     * @param   vehicleProperties  The properties to install for the edge.
     */
    void                setVehiclePropertyRestricionForEdge(
                            EdgeIdType edgeId,
                            VehicleProperties vehicleProperties);

// ACCESS
    /** Get which kinds of restrictions this edge has.
     * @param   edgeId      The edge to investigate.
     * @return  A vector with all types of restrictions.
     */
    std::vector<RestrictionType>
                        restrictionTypes(EdgeIdType edgeId) const;

    /** Try to fetch the vehicle property restrictions for an Edge.
     * @param   edgeId      The Edge to fetch restrictions for.
     * @return  The Vehicle properties
     * @throw   RestrictionException if no entry exists for Edge.
     */
    const VehicleProperties&
                        vehicleProperties(EdgeIdType edgeId) const;
    VehicleProperties&  vehicleProperties(EdgeIdType edgeId);

// INQUIRY
    /** Ask if an Edge has restriction of a certain type.
     * @param   edgeId              The edge in interest.
     * @param   restrictionType     The type of restriction
     * @return  true if there is a restriction of that type, false if not.
     */
    bool                hasRestriction(
                            EdgeIdType edgeId,
                            RestrictionType restrictionType) const;
    /**
     * @return true if there is a VehicleProperty restriction for edge.
     */
    bool                hasVehiclePropertyRestriction(EdgeIdType edgeId) const;

protected:
private:
    std::map<EdgeIdType, VehicleProperties>         mVehiclePropertiesMap;
    std::map<EdgeIdType, OsmAccess>                 mGeneralAccessMap;
    std::map<EdgeIdType, std::map<OsmVehicle::VehicleType, OsmAccess> >
                                                    mVehicleTypeAccessMap;
    std::map<EdgeIdType, OsmBarrier>                mBarrierMap;
    // from edge id => turning restriction
    std::map<EdgeIdType, OsmTurningRestriction>     mTurnRestrictionsMap;
    std::map<EdgeIdType, bool>                      mDisusedMap;
    std::map<EdgeIdType, bool>                      mNoExitMap;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//



#endif /* GRAPH_RESTRICTIONS_H_ */
