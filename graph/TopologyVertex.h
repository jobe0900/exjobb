/**  Data structure for vertices in Topology.
 *
 * #include "TopologyNode.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_TOPOLOGYVERTEX_H_
#define GRAPH_TOPOLOGYVERTEX_H_

// SYSTEM INCLUDES
//
#include <ostream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "../util/Point.h"

// FORWARD REFERENCES
//

typedef int VertexId;

/**
 * Data structure for vertices in the topology.
 */
class TopologyVertex
{
public:
// LIFECYCLE
	/** Constructor.
	 * @param	id		Id for this vertex.
	 * @param	point	The Point (geometry).
	 */
	TopologyVertex(int id, Point point);
	TopologyVertex() = delete;
	TopologyVertex(const TopologyVertex&) = default;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const TopologyVertex& rVertex);

	bool			operator==(const TopologyVertex& rhs) const;

// OPERATIONS
// ACCESS
	VertexId	id() const;
	Point		point() const;

	//INQUIRY

private:
	VertexId	mId;
	Point		mPoint;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYVERTEX_H_ */
