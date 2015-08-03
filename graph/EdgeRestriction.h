/**  The EdgeRestriction class contains different restrictions for edges
 * in the graph such as dimensions, access, turn restrictions.
 *
 * #include "EdgeRestriction.h"
 *
 * @author  Jonas Bergman
 */

#ifndef GRAPH_EDGERESTRICTION_H_
#define GRAPH_EDGERESTRICTION_H_

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
#include "../config/VehicleConfig.h"

// FORWARD REFERENCES
//
//class OsmTurningRestriction;

/**
 * EdgeRestriction are:
 * - vehicle properties
 * - General access to an edge
 * - vehicle type specific access
 * - barriers
 * - turn restrictions
 * - disused roads
 * - no-exit roads
 */
class EdgeRestriction
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

        /** Look if the vehicle properties restricts
         * vehicle with given configuration.
         */
        bool    restrictsAccess(const VehicleConfig& rVehicleConfig) const
        {
            return (maxHeight <= rVehicleConfig.height)
                || (maxLength <= rVehicleConfig.length)
                || (maxWeight <= rVehicleConfig.weight)
                || (maxWidth  <= rVehicleConfig.width)
                || (minSpeed  >= rVehicleConfig.maxspeed);
        }
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
    EdgeRestriction() = default;


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    EdgeRestriction(const EdgeRestriction& from) = delete;


    /** Destructor.
     */
    ~EdgeRestriction();


// OPERATORS
// OPERATIONS
    /** Set vehicle properties for the specified edge.
     * Replacing any existing properties with the new ones.
     * @param   pVehicleProperties  The properties to install for the edge.
     */
    void                setVehiclePropertyRestriction(
                            VehicleProperties* pVehicleProperties);

    /** Set access restrictions for this edge, that is restrictions for all.
     * @param   pGeneralAccess  The access to set.
     */
    void                setGeneralAccessRestriction(
                            OsmAccess*  pGeneralAccess);

    /** Set access restrictions for this edge, that is restrictions for all.
     * @param   generalAccessType  The access type to set.
     */
    void                setGeneralAccessRestriction(
                            OsmAccess::AccessType  generalAccessType);

    /** Set access restrictions for edge based on vehicle type.
     * There can be several vehicle restrictions for each edge.
     * @param   vehicleType The type of vehicle to restrict on the edge.
     * @param   pAccess     The access restriction for that vehicle type
     *                      on this edge.
     */
    void                addVehicleTypeAccessRestriction(
                            OsmVehicle::VehicleType  vehicleType,
                            OsmAccess*               pAccess);

    /** Set access restrictions for edge based on vehicle type.
     * There can be several vehicle restrictions for each edge.
     * @param   vehicleType The type of vehicle to restrict on the edge.
     * @param   accessType  The access restriction for that vehicle type
     *                      on this edge.
     */
    void                addVehicleTypeAccessRestriction(
                            OsmVehicle::VehicleType  vehicleType,
                            OsmAccess::AccessType    accessType);

    /** Set barrier restricting this edge.
     * @param   pBarrier        The barrier to set.
     */
    void                setBarrierRestriction(
                            OsmBarrier* pBarrier);

    /** Set barrier restricting this edge.
     * @param   barrierType    The barrier type to set.
     */
    void                setBarrierRestriction(
                            OsmBarrier::BarrierType barrierType);

    /** Add turning restrictions from this edge.
     * Actually just adds the restriction without checking if there already is
     * a restriction between those two edges.
     * @param   pTurningRestriction  The turning restriction to set.
     */
    void                addTurningRestriction(
                            OsmTurningRestriction* pTurningRestriction);

    /** Set disused flag on this edge.
     */
    void                setDisusedRestriction();

    /** Set no exit flag on this edge.
     */
    void                setNoExitRestriction();

    /** Flag this edge as part of a via way restriction that needs attention
     * when routing.
     */
    void                setViaWayRestriction();

// ACCESS
    /** Get which kinds of restrictions this edge has.
     * @return  A vector with all types of restrictions.
     */
    std::vector<RestrictionType>
                        restrictionTypes() const;

    /** Try to fetch the vehicle property restrictions for an Edge.
     * @return  The Vehicle properties
     * @throw   RestrictionException if no entry exists for Edge.
     */
    const VehicleProperties&
                        vehicleProperties() const;
    VehicleProperties&  vehicleProperties();

    /** Fetch the max speed for this edge. If no explicitcit speed is set it
     * returns `VehicleProperties::DEFAULT_SPEED_MAX`. One can query to see if
     * if there exists an explicit limit with  `hasMaxSpeedRestriction()`
     * @return  Either the explicit speed limit or a default if not set.
     */
    unsigned            maxSpeed() const;

//    /** Get the map of all VehicleProperty Restrictions.
//     * @return  Map of all property restrictions.
//     */
//    const std::map<EdgeIdType, VehicleProperties>&
//                        vehicleProperties() const;

    /** Try to fetch the general access restrictions for this edge.
     * @param   edgeId  The id of the edge.
     * @return  reference to the OsmAccess object.
     * @throw   RestrictionException if no entry exists for Edge.
     */
    const OsmAccess&    generalAccess() const;
    OsmAccess&          generalAccess();

//    /** Get all the general access restrictions.
//     * @return map of edges with barriers.
//     */
//    const std::map<EdgeIdType, OsmAccess>&
//                        generalAccess() const;

    /** Try to fetch the vehicle type specific access restrictions for this edge.
     * @param   vehiceltType    The type of Vehicle to get access restriction
     * @return  reference to the OsmAccess object.
     * @throw   RestrictionException if no entry exists for Edge.
     */
    const OsmAccess&    vehicleTypeAccess(
                            OsmVehicle::VehicleType vehicleType) const;
    OsmAccess&          vehicleTypeAccess(
                            OsmVehicle::VehicleType vehicleType);

    /** Get a list of the types of vehicles with restrictions on this edge.
     * @return  a Vector with restriction types.
     */
    std::vector<OsmVehicle::VehicleType>
                        vehicleTypesWithRestrictions() const;

//    /**
//     * @return Get the map of vehicle type restrictions for edges.
//     */
//    const std::map<EdgeIdType, std::map<OsmVehicle::VehicleType, OsmAccess> >&
//                        vehicleTypeAccessEdges() const;

    /** Fetch the barrier restricting this edge.
     * @return  reference to a OsmBarrier object.
     * @throw   RestrictionException if no entry exists for this Edge.
     */
    const OsmBarrier&   barrier() const;

//    /** Get all the barrier restrictions.
//     * @return map of edges with barriers.
//     */
//    const std::map<EdgeIdType, OsmBarrier>&
//                        barriers() const;

    /** Get a list of the turning restrictions from this edge.
     * @return  a Vector with turning restrictions.
     * @throw   RestrictionException if edge has no turning restrictions.
     */
    const std::vector<OsmTurningRestriction*>&
                        turningRestrictions() const;

    /** Get a list of all edge id's to which travel from edge is not allowed.
     * @return  A vector of edgeIds to which travel is not allowed.
     */
    std::vector<EdgeIdType>
                        restrictedTargetEdges() const;

//    /** Get all edges marked as disused.
//     * @return  The set of disused edges (ids).
//     */
//    const std::set<EdgeIdType>&
//                        disusedEdges() const;
//
//    /** Get all edges marked as 'noexit'.
//     * @return  The set of 'noexit' edges (ids).
//     */
//    const std::set<EdgeIdType>&
//                        noExitEdges() const;
// INQUIRY

    /** Check the restrictions for an edge.
     * @param   rVehicleConfig  Configuration for the current vehicle.
     * @param   rBarrierRule    Rules for which Barrier types restricts access.
     * @param   rAccessRule     Rules for which Access types restricts access.
     * @return  true if access is allowed, false if access restricted
     */
    bool                isEdgeRestricted(
        const VehicleConfig& rVehicleConfig,
        const OsmBarrier::RestrictionsRule& rBarrierRule,
        const OsmAccess::AccessRule& rAccessRule) const;

    /** Ask if an Edge has restriction of a certain type.
     * @param   restrictionType     The type of restriction
     * @return  true if there is a restriction of that type, false if not.
     */
    bool                hasRestriction(
                            RestrictionType restrictionType) const;
    /**
     * @return true if there is a VehicleProperty restriction for edge.
     */
    bool                hasVehiclePropertyRestriction() const;

    /** Convenience method to query for max speed.
     * @return true if there is a max speed restriction for the edge.
     */
    bool                hasMaxSpeedRestriction() const;

    /**
     * @return true if there is a General Access restriction for the edge.
     */
    bool                hasGeneralAccessRestriction() const;

    /**
     * @return true if there are any Vehicle Type Access restrictions for the edge.
     */
    bool                hasVehicleTypeAccessRestriction() const;

    /**
     * @return true if there are Vehicle Type Access restrictions for the edge
     *         for that specific type of vehicle.
     */
    bool                hasVehicleTypeAccessRestriction(
                            OsmVehicle::VehicleType vehicleType) const;

    /**
     * @return true if there are any barriers restricting access to the edge.
     */
    bool                hasBarrierRestriction() const;

    /**
     * @return true if there are any turning restrictions traveling from edge.
     */
    bool                hasTurningRestriction() const;

    /**
     * @return true if the edge is 'disused'.
     */
    bool                hasDisusedRestriction() const;

    /**
     * @return true if the edge has no exit.
     */
    bool                hasNoExitRestriction() const;

    /**
     * @return true if the edge is part of a turning restriction via another way.
     */
    bool               hasViaWayRestriction() const;


protected:
private:
    VehicleProperties*      mpVehicleProperties {nullptr};
    OsmAccess*              mpGeneralAccess {nullptr};
    std::map<OsmVehicle::VehicleType,  OsmAccess*>
                            mVehicleTypeAccessMap;
    OsmBarrier*             mpBarrier {nullptr};
    std::vector<OsmTurningRestriction*>
                            mTurningRestrictions;
    bool                    mIsDisusedEdge {false};
    bool                    mIsNoExitEdge {false};
    bool                    mHasViaWayRestriction {false};
//    std::map<EdgeIdType, VehicleProperties>         mVehiclePropertiesMap;
//    std::map<EdgeIdType, OsmAccess>                 mGeneralAccessMap;
//    std::map<EdgeIdType, std::map<OsmVehicle::VehicleType, OsmAccess> >
//                                                    mVehicleTypeAccessMap;
//    std::map<EdgeIdType, OsmBarrier>                mBarrierMap;
    // from edge id => turning restriction to several target edges
//    std::map<EdgeIdType, std::vector<OsmTurningRestriction> >
//                                                    mTurningRestrictionsMap;
//    std::set<EdgeIdType>                            mDisusedEdges;
//    std::set<EdgeIdType>                            mNoExitEdges;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGERESTRICTION_H_ */
