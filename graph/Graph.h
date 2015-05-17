/**  Graph.
 *
 * #include "Graph.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_GRAPH_H_
#define GRAPH_GRAPH_H_

// SYSTEM INCLUDES
//
#include <map>
//#include <unordered_map>

// PROJECT INCLUDES
//
#include <boost/graph/adjacency_list.hpp>

// LOCAL INCLUDES
//
#include "GraphException.h"
#include "TopologyTypes.h"
#include "Vertex.h"
#include "Edge.h"
#include "Topology.h"

// FORWARD REFERENCES
//

//typedef boost::adjacency_list
//    <boost::listS, boost::vecS, boost::undirectedS>    UndirectedGraphType;

typedef boost::adjacency_list
    <boost::listS, boost::vecS, boost::directedS>      DirectedGraphType;

//typedef boost::adjacency_list
//    <boost::listS, boost::vecS, boost::bidirectionalS> BidirectedGraphType;


/**
 * A class for a graph representing the topology.
 */
//template <typename GraphType>
class Graph
{
public:
    typedef DirectedGraphType                                   GraphType;
    typedef boost::graph_traits<GraphType>::vertex_descriptor   VertexType;
    typedef boost::graph_traits<GraphType>::edge_descriptor     EdgeType;

    typedef std::map<VertexIdType, VertexType>                  IdToGraphVertexMapType;
    typedef std::map<VertexType, VertexIdType>                  GraphVertexToIdMapType;
    typedef std::multimap<EdgeIdType, EdgeType>                 IdToGraphEdgeMapType;
    typedef std::map<EdgeType, EdgeIdType>                      GraphEdgeToIdMapType;

// LIFECYCLE
    Graph() = delete;
    /**
     * @throws GraphException if something is wrong with topology.
     */
    Graph(const Topology& rTopology);
    ~Graph() = default;
// OPERATORS
// OPERATIONS
// ACCESS
    size_t              nrVertices() const;
    size_t              nrEdges() const;
    const GraphType&    getRepresentation() const;
// INQUIRY
    bool                hasVertex(VertexIdType vertexId) const;

protected:

private:
// HELPERS
    void                addTopoVerticesToGraph();
    void                addTopoEdgesToGraph();
    void                addDirectedEdge(EdgeIdType id,
                                        const VertexType& source,
                                        const VertexType& target);
    const VertexType&   getGraphVertex(VertexIdType id) const;
// ATTRIBUTES
    GraphType               mGraph;
    IdToGraphVertexMapType  mIdToVertexMap;     // map original id to Vertex
    IdToGraphEdgeMapType    mIdToEdgeMap;       // map original id to Edge
    GraphVertexToIdMapType  mVertexToIdMap;     // map Vertex to original id
    GraphEdgeToIdMapType    mEdgeToIdMap;       // map Edge to original id
    const Topology&         mrTopology;
// CONSTANTS

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYGRAPH_H_ */
