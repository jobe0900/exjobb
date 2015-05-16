/**  Graph describing the topology.
 *
 * #include "TopologyGrap.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_TOPOLOGYGRAPH_H_
#define GRAPH_TOPOLOGYGRAPH_H_

// SYSTEM INCLUDES
//
#include <map>

// PROJECT INCLUDES
//
#include <boost/graph/adjacency_list.hpp>

#include "Edge.h"
// LOCAL INCLUDES
//
#include "TopologyException.h"
#include "TopologyTypes.h"
#include "Vertex.h"
//#include "Topology.h"

// FORWARD REFERENCES
//

struct VertexData
{
    VertexIdType    topo_id;
    double      x;
    double      y;

    VertexData() : topo_id(0), x(0.0), y(0.0) {}
    VertexData(VertexIdType id, double x, double y) : topo_id(id), x(x), y(y) {}
};

struct EdgeData
{
    EdgeIdType      topo_id;
    VertexIdType    source;
    VertexIdType    target;

    EdgeData() : topo_id(0), source(0), target(0) {}
    EdgeData(EdgeIdType id, VertexIdType source, VertexIdType target)
        : topo_id(id), source(source), target(target) {}
};


/**
 * A class for a graph representing the topology.
 */
class TopologyGraph
{
public:
    typedef boost::adjacency_list
        < boost::listS, boost::vecS, boost::bidirectionalS,
          VertexData, EdgeData >                                GraphType;
    typedef boost::graph_traits<GraphType>::vertex_descriptor   VertexType;
    typedef boost::graph_traits<GraphType>::edge_descriptor     EdgeType;
// LIFECYCLE
    TopologyGraph();
    TopologyGraph(size_t nrVertices);
    ~TopologyGraph();
// OPERATORS
// OPERATIONS
    void                addVertex(VertexData vertex);
    void                addEdge(EdgeData edge);
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
    GraphType                       mGraph;
    std::map<VertexIdType, VertexType>  mVertexMap;     // map original id to Vertex
    std::map<EdgeIdType, EdgeType>      mEdgeMap;       // map original id to Edge
// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYGRAPH_H_ */
