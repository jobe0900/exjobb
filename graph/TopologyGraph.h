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

// LOCAL INCLUDES
//
#include "TopologyVertex.h"
#include "TopologyEdge.h"
#include "TopologyException.h"
//#include "Topology.h"

// FORWARD REFERENCES
//


/**
 * A class for a graph representing the topology.
 */
class TopologyGraph
{
public:
    typedef boost::adjacency_list
        < boost::listS, boost::vecS, boost::bidirectionalS,
          TopologyVertex, TopologyEdge >                        GraphType;
    typedef boost::graph_traits<GraphType>::vertex_descriptor   VertexType;
    typedef boost::graph_traits<GraphType>::edge_descriptor     EdgeType;
// LIFECYCLE
    TopologyGraph();
    TopologyGraph(size_t nrVertices);
    ~TopologyGraph();
// OPERATORS
// OPERATIONS
//    void                addVertex(const TopologyVertex* pVertex);
    void                addVertex(TopologyVertex vertex);
//    void    addEdge(const TopologyEdge* pEdge);
    void                addEdge(TopologyEdge edge);
// ACCESS
    size_t              nrVertices() const;
    size_t              nrEdges() const;
    const GraphType&    getRepresentation() const;
// INQUIRY
    bool                hasVertex(VertexId vertexId) const;

protected:

private:
// HELPERS
// ATTRIBUTES
    GraphType                       mGraph;
    std::map<VertexId, VertexType>  mVertexMap;     // map original id to Vertex
    std::map<EdgeId, EdgeType>      mEdgeMap;       // map original id to Edge
//    size_t                          mNrVertices;
//    size_t                          mNrEdges;
// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYGRAPH_H_ */
