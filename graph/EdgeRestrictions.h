/**  The EdgeRestrictions class contains different restrictions for edges
 * in the graph such as dimensions, access, turn restrictions.
 *
 * #include "EdgeRestrictions.h" <BR>
 *
 * @author  Jonas Bergman
 */

#ifndef GRAPH_EDGERESTRICTIONS_H_
#define GRAPH_EDGERESTRICTIONS_H_

// SYSTEM INCLUDES
//
#include <limits>
#include <map>
#include <set>

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
 * EdgeRestrictions maps are:
 * - vehicle properties
 * - General access to an edge
 * - vehicle type specific access
 * - barriers
 * - turn restrictions
 * - disused roads
 * - no-exit roads
 */
class EdgeRestrictions
{
public:
// TYPES
    /** EdgeRestrictions on Vehicles to travel an Edge.
     *  Dimensions in meters.
     *  Speed in km/h
     */
    struct VehicleProperties
    {
        static double DEFAULT_DIMENSION_MAX;
        // = {std::numeric_limits<double>::max()};

        static unsigned DEFAULT_SPEED_MAX;
        // = {std::numeric_limits<unsigned>::max()};

        static unsigned DEFAULT_SPEED_MIN;
        // = {0};

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
    EdgeRestrictions();


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    EdgeRestrictions(const EdgeRestrictions& from) = default;


    /** Destructor.
     */
    ~EdgeRestrictions() = default;


// OPERATORS
// OPERATIONS
    /** Set vehicle properties for the specified edge.
     * Replacing any existing properties with the new ones.
     * @param   edgeId              Id of the edge.
     * @param   vehicleProperties  The properties to install for the edge.
     */
    void                setVehiclePropertyRestrictionForEdge(
                            EdgeIdType edgeId,
                            VehicleProperties vehicleProperties);

    /** Set access restrictions for this edge, that is restrictions for all.
     * @param   edgeId         The id of the edge to apply restrictions to.
     * @param   generalAccess  The access type to set.
     */
    void                setGeneralAccessRestrictionForEdge(
                            EdgeIdType edgeId,
                            OsmAccess  generalAccess);

    /** Set access restrictions for edge based on vehicle type.
     * There can be several vehicle restrictions for each edge.
     * The mapping is in two levels:
     * EdgeId => (VehicleType => Access)
     * @param   edgeId      The id of the edge to set restrictions for.
     * @param   vehicleType The type of vehicle to restrict on the edge.
     * @param   access      The access restriction for that vehicle type
     *                      on this edge.
     */
    void                addVehicleTypeAccessRestrictionsForEdge(
                            EdgeIdType               edgeId,
                            OsmVehicle::VehicleType  vehicleType,
                            OsmAccess                access);

    /** Set barrier restricting this edge.
     * @param   edgeId         The id of the edge to apply restrictions to.
     * @param   barrier        The barrier type to set.
     */
    void                setBarrierRestrictionForEdge(
                            EdgeIdType edgeId,
                            OsmBarrier barrier);

    /** Add turning restrictions from this edge.
     * Actually just adds the restriction without checking if there already is
     * a restriction between those two edges.
     * @param   edgeId              The id of the edge to apply restrictions to.
     * @param   turningRestriction  The barrier type to set.
     */
    void                addTurningRestrictionForEdge(
                            EdgeIdType            edgeId,
                            OsmTurningRestriction turningRestriction);

    /** Set disused flag on this edge.
     * @param   edgeId  The id of the edge to apply restrictions to.
     */
    void                setDisusedRestrictionForEdge(EdgeIdType edgeId);

    /** Set no exit flag on this edge.
     * @param   edgeId  The id of the edge to apply restrictions to.
     */
    void                setNoExitRestrictionForEdge(EdgeIdType edgeId);

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

    /** Get the map of all VehicleProperty Restrictions.
     * @return  Map of all property restrictons.
     */
    const std::map<EdgeIdType, VehicleProperties>&
                        vehicleProperties() const;

    /** Try to fetch the general access restrictions for this edge.
     * @param   edgeId  The id of the edge.
     * @return  reference to the OsmAccess object.
     * @throw   RestrictionException if no entry exists for Edge.
     */
    const OsmAccess&    generalAccess(EdgeIdType edgeId) const;
    OsmAccess&          generalAccess(EdgeIdType edgeId);

    /** Try to fetch the vehicle type specific access restrictions for this edge.
     * @param   edgeId          The id of the edge.
     * @param   vehiceltType    The type of Vehicle to get access restriction
     * @return  reference to the OsmAccess object.
     * @throw   RestrictionException if no entry exists for Edge.
     */
    const OsmAccess&    vehicleTypeAccess(
                            EdgeIdType edgeId,
                            OsmVehicle::VehicleType vehicleType) const;
    OsmAccess&          vehicleTypeAccess(
                            EdgeIdType edgeId,
                            OsmVehicle::VehicleType vehicleType);

    /** Get a list of the types of vehicles with restrictions on this edge.
     * @param   edgeId  The id of the Edge.
     * @return  a Vector with restriction types.
     */
    std::vector<OsmVehicle::VehicleType>
                        vehicleTypesWithRestrictions(EdgeIdType edgeId) const;

    /** Fetch the barrier restricting this edge.
     * @param   edgeId  The id of the edge.
     * @return  reference to a OsmBarrier object.
     * @throw   RestrictionException if no entry exists for this Edge.
     */
    const OsmBarrier&   barrier(EdgeIdType edgeId) const;

    /** Get a list of the turning restrictions from this edge.
     * @param   edgeId  The id of the Edge.
     * @return  a Vector with restriction types.
     * @throw   RestrictionException if edge has no turning restrictions.
     */
    const std::vector<OsmTurningRestriction>&
                        turningRestrictions(EdgeIdType edgeId) const;
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

    /**
     * @return true if there is a General Access restriction for the edge.
     */
    bool                hasGeneralAccessRestriction(EdgeIdType edgeId) const;

    /**
     * @return true if there are any Vehicle Type Access restrictions for the edge.
     */
    bool                hasVehicleTypeAccessRestriction(EdgeIdType edgeId) const;

    /**
     * @return true if there are Vehicle Type Access restrictions for the edge
     *         for that specific type of vehicle.
     */
    bool                hasVehicleTypeAccessRestriction(
                            EdgeIdType edgeId,
                            OsmVehicle::VehicleType vehicleType) const;

    /**
     * @return true if there are any barriers restricting access to the edge.
     */
    bool                hasBarrierRestriction(EdgeIdType edgeId) const;

    /**
     * @return true if there are any turning restrictions traveling from edge.
     */
    bool                hasTurningRestriction(EdgeIdType edgeId) const;

    /**
     * @return true if the edge is 'disused'.
     */
    bool                hasDisusedRestriction(EdgeIdType edgeId) const;

    /**
     * @return true if the edge has no exit.
     */
    bool                hasNoExitRestriction(EdgeIdType edgeId) const;

protected:
private:
    std::map<EdgeIdType, VehicleProperties>         mVehiclePropertiesMap;
    std::map<EdgeIdType, OsmAccess>                 mGeneralAccessMap;
    std::map<EdgeIdType, std::map<OsmVehicle::VehicleType, OsmAccess> >
                                                    mVehicleTypeAccessMap;
    std::map<EdgeIdType, OsmBarrier>                mBarrierMap;
    // from edge id => turning restriction to several target edges
    std::map<EdgeIdType, std::vector<OsmTurningRestriction> >
                                                    mTurningRestrictionsMap;
    std::set<EdgeIdType>                            mDisusedEdges;
    std::set<EdgeIdType>                            mNoExitEdges;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//



#endif /* GRAPH_EDGERESTRICTIONS_H_ */
