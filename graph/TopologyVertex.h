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
struct TopologyVertex
{
	int		mId;
	Point	mPoint;

	TopologyVertex(int id, Point point) : mId(id), mPoint(point) {}
	TopologyVertex() = delete;
//	TopologyVertex(const TopologyVertex&) = delete;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYVERTEX_H_ */
