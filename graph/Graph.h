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
#include <ostream>

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

// TYPES
/**
 * Map the GraphEdges to the original Edge id in the Topology.
 */
struct GraphEdge
{
    EdgeIdType  graphEdgeId;
    EdgeIdType  topoEdgeId;
};

/**
 * Map the GraphVertices to the original Vertex id in the Topology.
 */
struct GraphVertex
{
    VertexIdType  graphVertexId;
    VertexIdType  topoVertexId;
};



/**
 * A class for a graph representing the topology.
 */
//template <typename GraphType>
class Graph
{
public:
// TYPES
    typedef boost::adjacency_list
        < boost::listS, boost::vecS, boost::directedS,
          GraphVertex, GraphEdge >                              DirectedGraphType;
    typedef DirectedGraphType                                   GraphType;
    typedef boost::graph_traits<GraphType>::vertex_descriptor   VertexType;
    typedef boost::graph_traits<GraphType>::edge_descriptor     EdgeType;

    typedef std::map<VertexIdType, VertexType>                  IdToGraphVertexMapType;
    typedef std::multimap<EdgeIdType, EdgeType>                 IdToGraphEdgeMapType;

// LIFECYCLE
    /** Constructor.
     * Disabled.
     */
    Graph() = delete;

    /** Constructor.
     * Build a graph from the supplied topology.
     * @param   rTopology   The topology to use as basis for the graph.
     * @throws  GraphException if something is wrong with topology.
     */
    Graph(const Topology& rTopology);

    /** Copy constructor.
     * Disabled.
     */
    Graph(const Graph& from) = delete;

    /** Destructor.
     */
    ~Graph() = default;

// OPERATORS
    /** Output operator to print to a stream.
     */
    friend
    std::ostream&       operator<<(std::ostream& os, const Graph& rGraph);

// OPERATIONS
// ACCESS
    /**
     * @return  The number of vertices in the graph.
     */
    size_t              nrVertices() const;

    /**
     * @return  The number of edges in the graph.
     */
    size_t              nrEdges() const;

    /**
     * @return  The Boost Graph representation of the Graph.
     */
    const GraphType&    getRepresentation() const;

// INQUIRY
    /**
     * @return  true    If graph has a vertex with given index.
     */
    bool                hasVertex(VertexIdType vertexId) const;

protected:

private:
// HELPERS
    // Used when constructing the internal Boost graph representation
    // from the Topology.
    void                addTopoVerticesToGraph();
    void                addTopoEdgesToGraph();
    void                addDirectedEdge(EdgeIdType id,
                                        const VertexType& source,
                                        const VertexType& target,
                                        EdgeIdType ix);

    const VertexType&   getGraphVertex(VertexIdType id) const;

    void                printVertices(std::ostream& os) const;
    void                printEdges(std::ostream& os)    const;

// ATTRIBUTES
    GraphType               mGraph;
    IdToGraphVertexMapType  mIdToVertexMap;     // map original id to Vertex
    IdToGraphEdgeMapType    mIdToEdgeMap;       // map original id to Edge
    const Topology&         mrTopology;

// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYGRAPH_H_ */
