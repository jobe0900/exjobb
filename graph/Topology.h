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
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

// LOCAL INCLUDES
//
#include "TopologyException.h"
#include "../util/Point.h"
#include "Edge.h"
#include "Vertex.h"


// FORWARD REFERENCES
//

typedef std::map<VertexIdType, Vertex>  TopoVertexMapType;
typedef std::map<EdgeIdType, Edge>      TopoEdgeMapType;

class Topology
{
    friend class Graph;

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
//	const Vertex&   addVertex(VertexIdType id, Point point);
	Vertex&         addVertex(VertexIdType id, Point point);
//	const Vertex&   addVertex(Vertex vertex);


	/** Try to add an edge to the topology.
	 * If an edge with the id already exists: return old value.
	 * @param	id		Id for the edge
	 * @param	source	Id for source vertex
	 * @param	target	Id for target vertex
	 * @return	A reference to an edge with given id
	 * @throw	Topology Exception if vertices are not in topology.
	 */
//	const Edge&     addEdge(EdgeIdType id, VertexIdType source, VertexIdType target);
	Edge&           addEdge(EdgeIdType id, VertexIdType source, VertexIdType target);
//	const Edge&     addEdge(Edge edge);

	/** Fetch the vertex with given id.
	 * @param	id		Id of the vertex to get
	 * @return	Reference to the found vertex
	 * @throws	TopologyException if vertex does not exist.
	 */
	const Vertex&   getVertex(VertexIdType id) const;
	Vertex&         getVertex(VertexIdType id);

	const Edge&     getEdge(EdgeIdType id) const;
	Edge&           getEdge(EdgeIdType id);

// ACCESS
	/**
	 * @return	the Number of vertices in topology.
	 */
	size_t			nrVertices() const;

	/**
	 * @return	the Number of vertices in topology.
	 */
	size_t			nrEdges() const;

// INQUIRY

protected:
private:
// ATTRIBUTES
	TopoVertexMapType  vertexMap;
	TopoEdgeMapType    edgeMap;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGY_H_ */
