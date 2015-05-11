/**  A class holding the elements of the topology.
 *
 * #include "Topology.h"
 *
 * A longer description.
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_TOPOLOGY_H_
#define GRAPH_TOPOLOGY_H_

// SYSTEM INCLUDES
//
#include <map>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "TopologyEdge.h"
#include "TopologyVertex.h"
#include "../util/Point.h"


// FORWARD REFERENCES
//

typedef int TopologyId;

class Topology
{
public:
	// LIFECYCLE

	/** Default constructor.
	 */
	Topology();


	/** Copy constructor.
	 *
	 * @param from The value to copy to this object.
	 */
	Topology(const Topology& from) = delete;


	/** Destructor.
	 */
//	~Topology(void);


	// OPERATORS
	// OPERATIONS

	/** Try to add a vertex to the topology.
	 * If a vertex with the id already exists: return old value.
	 * @param	id		Id for the vertex
	 * @param	point	The position of the vertex
	 * @return	A reference to a vertex with given id
	 */
	const TopologyVertex&	addVertex(TopologyId id, Point point);

	// ACCESS
	// INQUIRY

protected:
private:
	std::map<TopologyId, TopologyVertex> mVertexMap;
	std::map<TopologyId, TopologyEdge> mEdgeMap;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGY_H_ */
