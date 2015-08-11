/**  The Costs for an Edge.
 * The cost or weight can be thought of as seconds, with the time to travel
 * the edge as a base, and different obstacles as additional costs.
 *
 * #include "EdgeCost.h"
 *
 * @author  Jonas Bergman
 */
#ifndef GRAPH_EDGECOST_H_
#define GRAPH_EDGECOST_H_

// SYSTEM INCLUDES
//
#include <map>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "Cost.h"

// FORWARD REFERENCES
//
//typedef double Cost;

/** Costs for edges:
 * - speed (either set explicitly or default  from road category)
 * - barriers (should be imported from the EdgeRestriction)
 * - incline
 * - surface
 * - traffic_calming
 * - highway => bus_stop
 * - highway => crossing
 * - highway => give_way
 * - highway => mini_roundabout
 * - highway => stop
 * - highway => traffic_signals
 * - public_transport => stop_position
 * - railway => level_crossing
 */
class EdgeCost
{
public:
// TYPES
    enum CostType
    {
        TRAVEL_TIME,
        BARRIER,
        INCLINE,
//        SURFACE,
        TRAFFIC_CALMING,
        BUS_STOP,
        CROSSING,
        GIVE_WAY,
        MINI_ROUNDABOUT,
        STOP,
        TRAFFIC_SIGNALS,
        RAILWAY_CROSSING
    };

// LIFECYCLE

    /** Default constructor.
     */
    EdgeCost() = default;


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    EdgeCost(const EdgeCost& from) = delete;


    /** Destructor.
     */
    ~EdgeCost() = default;


// OPERATORS

    /** Accumulate a cost of a certain type, except `travel` which can not
     * be accumulated.
     * @param   type    The type of cost
     * @param   cost    The value of the cost in seconds.
     */
    void    addCost(CostType type, Cost cost);

// OPERATIONS
// ACCESS

    /**
     * @return  The sum of all costs
     */
    Cost    getCost() const;

    /**
     * @return  The accumulated costs of a CostType
     */
    Cost    getCost(CostType type) const;

// INQUIRY
    /** Find out if there are costs of a certain type
     * @return  True if there is such a cost
     */
    bool    hasCost(CostType type) const;

protected:
private:
    std::map<CostType, Cost> costs;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGECOST_H_ */
