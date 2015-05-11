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
#include "TopologyException.h"
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

// OPERATORS
// OPERATIONS

	/** Try to add a vertex to the topology.
	 * If a vertex with the id already exists: return old value.
	 * @param	id		Id for the vertex
	 * @param	point	The position of the vertex
	 * @return	A reference to a vertex with given id
	 */
	const TopologyVertex&	addVertex(TopologyId id, Point point);

	/** Try to add an edge to the topology.
	 * If an edge with the id already exists: return old value.
	 * @param	id		Id for the edge
	 * @param	source	Id for source vertex
	 * @param	target	Id for target vertex
	 * @return	A reference to an edge with given id
	 * @throw	Topology Exception if vertices are not in topology.
	 */
	const TopologyEdge&		addEdge(EdgeId id, VertexId source, VertexId target);

	/** Fetch the vertex with given id.
	 * @param	id		Id of the vertex to get
	 * @return	Reference to the found vertex
	 * @throws	TopologyException if vertex does not exist.
	 */
	const TopologyVertex&	getVertex(VertexId id) const;

// ACCESS
	/**
	 * @return	the Number of vertices in topology.
	 */
	size_t					nrVertices() const;

	/**
	 * @return	the Number of vertices in topology.
	 */
	size_t					nrEdges() const;

// INQUIRY

protected:
private:
// ATTRIBUTES
	std::map<TopologyId, TopologyVertex> 	mVertexMap;
	std::map<TopologyId, TopologyEdge> 		mEdgeMap;

};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGY_H_ */
