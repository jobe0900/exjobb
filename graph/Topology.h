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
#include "TopologyTypes.h"
#include "TopologyException.h"
#include "../util/Point.h"
#include "../mapprovider/MapProvider.h"
#include "Edge.h"
#include "Vertex.h"


// FORWARD REFERENCES
//

typedef std::map<TopoIdType, Vertex>  TopoVertexMapType;
typedef std::map<TopoIdType, Edge>    TopoEdgeMapType;

class Topology
{
public:
// LIFECYCLE

	/** Default constructor.
	 */
	Topology();

	/** Constructor.
	 * @param rMapProvder   The provider of the map data topology.
	 */
//	Topology(const MapProvider& rMapProvider);

//    Topology(const TopoVertexMapType& rVertexMap,
//             const TopoEdgeMapType& rEdgeMap);


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
	const Vertex&   addVertex(VertexIdType id, Point point);

	/** Try to add an edge to the topology.
	 * If an edge with the id already exists: return old value.
	 * @param	id		Id for the edge
	 * @param	source	Id for source vertex
	 * @param	target	Id for target vertex
	 * @return	A reference to an edge with given id
	 * @throw	Topology Exception if vertices are not in topology.
	 */
	const Edge&     addEdge(EdgeIdType id, VertexIdType source, VertexIdType target);

	/** Fetch the vertex with given id.
	 * @param	id		Id of the vertex to get
	 * @return	Reference to the found vertex
	 * @throws	TopologyException if vertex does not exist.
	 */
	const Vertex&   getVertex(VertexIdType id) const;

	/** Build a simple graph over the topology.
	 * @param   rGraph  Reference to TopologyGraph to build
	 */
//	void                    buildTopologyGraph(TopologyGraph& rGraph);

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
	TopoVertexMapType  vertexMap;
	TopoEdgeMapType    edgeMap;
//// ATTRIBUTES
//	const std::map<TopologyId, Vertex>& 	mVertexMap;
//	const std::map<TopologyId, TopologyEdge>& 		mEdgeMap;

//	const MapProvider&                              mrMapProvider;


};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGY_H_ */
