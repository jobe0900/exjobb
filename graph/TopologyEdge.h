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
struct TopologyEdge
{
	EdgeId		mId;
	VertexId	mSource;
	VertexId	mTarget;

	TopologyEdge(EdgeId id, VertexId source, VertexId target)
		: mId(id), mSource(source), mTarget(target) {}
	TopologyEdge() = delete;
	TopologyEdge(const TopologyEdge&) = delete;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const TopologyEdge& rEdge)
	{
		os  << "TopologyEdge [id: " << rEdge.mId
			<< ", source: " << rEdge.mSource
			<< ", target: " << rEdge.mTarget << "]";
		return os;
	}

	bool			operator==(const TopologyEdge& rhs) const
	{
		return (rhs.mId == mId)
				&& (rhs.mSource == mSource)
				&& (rhs.mTarget == mTarget);
	}
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYEDGE_H_ */
