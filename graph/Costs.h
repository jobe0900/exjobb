#ifndef GRAPH_COSTS_H_
#define GRAPH_COSTS_H_

/**  The Costs class is a container for
 *   EdgeCosts and VertexCosts.
 *
 * #include "Costs.h"
 *
 * @author  Jonas Bergman
 */

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "EdgeRestrictions.h"
#include "RestrictionsException.h"
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

    const VertexCosts&   vertexRestrictions() const
                         { return mVertexCosts; }

    VertexCosts&         vertexCosts()
                         { return mVertexCosts; }

// INQUIRY
protected:
private:
    EdgeCosts    mEdgeCosts {};
    VertexCosts  mVertexCosts {};
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//
#endif /* GRAPH_COSTS_H_ */
