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
	TopologyEdge(EdgeId id, const TopologyVertex& source, const TopologyVertex& target)
		: mId(id), mrSource(source), mrTarget(target) {}
	TopologyEdge() = delete;
	TopologyEdge(const TopologyEdge&) = delete;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const TopologyEdge& rEdge)
	{
		os  << "TopologyEdge [id: " << id()
			<< ", source: " << rEdge.source().mId
			<< ", target: " << rEdge.target().mId << "]";
		return os;
	}

	bool			operator==(const TopologyEdge& rhs) const
	{
		return (rhs.id() == id())
				&& (rhs.source() == source())
				&& (rhs.target() == target());
	}
// OPERATIONS
// ACCESSORS
	EdgeId					id() const { return mId; }
	const TopologyVertex&	source() const { return mrSource; }
	const TopologyVertex&	target() const { return mrTarget; }
// INQUIRY

private:
	EdgeId					mId;
	const TopologyVertex&	mrSource;
	const TopologyVertex&	mrTarget;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYEDGE_H_ */
