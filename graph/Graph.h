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

typedef boost::adjacency_list
    <boost::listS, boost::vecS, boost::undirectedS>    UndirectedGraphType;

typedef boost::adjacency_list
    <boost::listS, boost::vecS, boost::directedS>      DirectedGraphType;

typedef boost::adjacency_list
    <boost::listS, boost::vecS, boost::bidirectionalS> BidirectedGraphType;


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
    typedef std::map<VertexIdType, VertexType>                  GraphVertexMapType;
    typedef std::map<EdgeIdType, EdgeType>                      GraphEdgeMapType;

// LIFECYCLE
    Graph() = delete;
    Graph(const Topology& rTopology);
//    Graph(size_t nrVertices);
    ~Graph();
// OPERATORS
// OPERATIONS
    void                addVertex(const Vertex& vertex);
    void                addEdge(const Edge& edge);
// ACCESS
    size_t              nrVertices() const;
    size_t              nrEdges() const;
    const GraphType&    getRepresentation() const;
// INQUIRY
    bool                hasVertex(VertexIdType vertexId) const;

protected:

private:
// HELPERS
// ATTRIBUTES
    GraphType           mGraph;
    GraphVertexMapType  mVertexMap;     // map original id to Vertex
    GraphEdgeMapType    mEdgeMap;       // map original id to Edge
    const Topology&     mrTopology;
// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYGRAPH_H_ */
