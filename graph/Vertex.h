/**  Data structure for vertices in Topology.
 *
 * #include "TopologyNode.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_VERTEX_H_
#define GRAPH_VERTEX_H_

// SYSTEM INCLUDES
//
#include <ostream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "../util/Point.h"
#include "TopologyTypes.h"

// FORWARD REFERENCES
//

/**
 * Data structure for vertices in the topology.
 */
class Vertex
{
public:
// LIFECYCLE
	/** Constructor.
	 * @param	id		Id for this vertex.
	 * @param	point	The Point (geometry).
	 */
	Vertex(VertexId id, Point point);
	Vertex() = delete;
	Vertex(const Vertex&) = default;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const Vertex& rVertex);

	bool			operator==(const Vertex& rhs) const;

// OPERATIONS
// ACCESS
	VertexId	id() const;
	Point		point() const;

//INQUIRY

private:
// ATTRIBUTES
	VertexId	mId;
	Point		mPoint;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_VERTEX_H_ */
