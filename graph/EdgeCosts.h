/**  The Costs for an Edge
 *
 * #include "EdgeCosts.h"
 *
 * @author  Jonas Bergman
 */
#ifndef GRAPH_EDGECOSTS_H_
#define GRAPH_EDGECOSTS_H_

// SYSTEM INCLUDES
//
#include <map>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "Edge.h"
#include "Vertex.h"
#include "../osm/OsmBarrier.h"

// FORWARD REFERENCES
//

/** Costs for edges other than length:
 * - barriers (should be imported from the EdgeRestrictions)
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
class EdgeCosts
{
// TYPES
    enum CostType
    {
        BARRIER,
        INCLINE,
        SURFACE,
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
    EdgeCosts() = default;


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    EdgeCosts(const EdgeCosts& from) = default;


    /** Destructor.
     */
    ~EdgeCosts() = default;


// OPERATORS
// OPERATIONS
// ACCESS
// INQUIRY
protected:
private:
    std::map<EdgeIdType, OsmBarrier>    mBarrierMap;
    std::map<EdgeIdType, int>           mInclinePercentMap;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGECOSTS_H_ */
