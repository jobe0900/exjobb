/**  The Costs class is a container for
 *   EdgeCosts and TurnCosts.
 *
 * #include "Costs.h"
 *
 * @author  Jonas Bergman
 */
#ifndef GRAPH_COSTS_H_
#define GRAPH_COSTS_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "EdgeCosts.h"
#include "Edge.h"
#include "Vertex.h"

// FORWARD REFERENCES
//

/**
 * A container for EdgeCosts and VertexCosts.
 */
class Costs
{
public:
// TYPES
// LIFECYCLE

    /** Default constructor.
     */
    Costs() = default;


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    Costs(const Costs& from) = default;


    /** Destructor.
     */
    ~Costs() = default;


// OPERATORS
// OPERATIONS
// ACCESS
    const EdgeCosts&     edgeCosts() const
                         { return mEdgeCosts; }

    EdgeCosts&           edgeCosts()
                         { return mEdgeCosts; }

//    const TurnCosts&     turnCosts() const
//                         { return mTurnCosts; }
//
//    TurnCosts&           turnCosts()
//                         { return mTurnCosts; }

// INQUIRY
protected:
private:
    EdgeCosts    mEdgeCosts {};
//    TurnCosts    mTurnCosts {};
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//
#endif /* GRAPH_COSTS_H_ */
