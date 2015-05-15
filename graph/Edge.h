/**  Data structure for edges in Topology.
 *
 * #include "Edge.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_EDGE_H_
#define GRAPH_EDGE_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "TopologyTypes.h"
#include "Vertex.h"

// FORWARD REFERENCES
//


/**
 * Data structure for edges in the topology.
 */
class Edge
{
public:
// LIFECYCLE
	/** Constructor.
	 * @param	id		Id for this Edge
	 * @param	source	Source vertex
	 * @param	target	Target vertex
	 */
	Edge(EdgeId id, VertexId source, VertexId target);
//	Edge() = delete;
	Edge(const Edge&) = default;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const Edge& rEdge);

	bool			operator==(const Edge& rhs) const;

// OPERATIONS
// ACCESSORS
	EdgeId		id() const;
	VertexId	source() const;
	VertexId	target() const;
// INQUIRY

private:
// ATTRIBUTES
	EdgeId		mId;
	VertexId	mSource;
	VertexId	mTarget;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGE_H_ */
