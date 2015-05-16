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
#include "Vertex.h"

// FORWARD REFERENCES
//
typedef unsigned        EdgeIdType;

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
	Edge(EdgeIdType id, VertexIdType source, VertexIdType target);
//	Edge() = delete;
	Edge(const Edge&) = default;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const Edge& rEdge);

	bool			operator==(const Edge& rhs) const;

// OPERATIONS
// ACCESSORS
	EdgeIdType		id() const;
	VertexIdType	source() const;
	VertexIdType	target() const;
// INQUIRY

private:
// ATTRIBUTES
	EdgeIdType		mId;
	VertexIdType	mSource;
	VertexIdType	mTarget;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGE_H_ */
