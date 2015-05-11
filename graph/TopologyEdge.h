/**  Data structure for edges in Topology.
 *
 * #include "TopologyEdge.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_TOPOLOGYEDGE_H_
#define GRAPH_TOPOLOGYEDGE_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "TopologyVertex.h"

// FORWARD REFERENCES
//

typedef int EdgeId;

/**
 * Data structure for edges in the topology.
 */
class TopologyEdge
{
public:
// LIFECYCLE
	/** Constructor.
	 * @param	id		Id for this Edge
	 * @param	source	Source vertex
	 * @param	target	Target vertex
	 */
	TopologyEdge(EdgeId id, VertexId source, VertexId target);
	TopologyEdge() = delete;
	TopologyEdge(const TopologyEdge&) = default;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const TopologyEdge& rEdge);

	bool			operator==(const TopologyEdge& rhs) const;

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

#endif /* GRAPH_TOPOLOGYEDGE_H_ */
