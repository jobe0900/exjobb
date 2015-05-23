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
typedef EdgeIdType  NodeIdType;
typedef EdgeIdType  LineIdType;
/**
 * Map the GraphEdges to the original Edge id in the Topology.
 */
struct GraphEdge
{
    EdgeIdType      graphEdgeId;
    EdgeIdType      topoEdgeId;
};

/**
 * A Node in the LineGraph corresponds directly to an Edge in the original
 * Graph and topology. It is connected to another Node (Edge) if both the
 * edges are adjacent and there is no restriction in the Vertex for travel
 * along them.
 * lgNodeId === graphEdgeId
 */
typedef GraphEdge   LineGraphNode;

/**
 * Map the GraphVertices to the original Vertex id in the Topology.
 */
struct GraphVertex
{
    VertexIdType    graphVertexId;
    VertexIdType    topoVertexId;
};

/**
 * A LineGraphLine corresponds to a travel along an incoming edge,
 * via a vertex and out an outgoing edge.
 * The cost is the cost of travel on the incoming edge and the turn cost
 * at the vertex.
 * The Line connects two edges in the graph with an allowed turn in between.
 */
struct LineGraphLine
{
    NodeIdType      lgSourceNodeId;
    NodeIdType      lgTargetNodeId;
    VertexIdType    topoViaVertexId;
    double          cost;
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
          GraphVertex, GraphEdge >                      GraphType;
    typedef boost::adjacency_list
        < boost::listS, boost::vecS, boost::directedS,
          LineGraphNode, LineGraphLine >                LineGraphType;

    typedef boost::graph_traits<GraphType>
                                ::vertex_descriptor     VertexType;
    typedef boost::graph_traits<GraphType>
                                ::edge_descriptor       EdgeType;

    typedef boost::graph_traits<LineGraphType>
                                ::vertex_descriptor     NodeType;
    typedef boost::graph_traits<LineGraphType>
                                ::edge_descriptor       LineType;

    typedef std::map<VertexIdType, VertexType>          TopoVertexIdToGraphVertexMapType;
    typedef std::multimap<EdgeIdType, EdgeType>         TopoEdgeIdToGraphEdgeMapType;
    typedef std::map<EdgeIdType, NodeType>              GraphEdgeIdToNodeMapType;


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
    std::ostream&         operator<<(std::ostream& os, const Graph& rGraph);

// OPERATIONS
// ACCESS
    /**
     * @return  The number of Vertices in the Graph.
     */
    size_t                nrVertices() const;

    /**
     * @return  The number of Edges in the Graph.
     */
    size_t                nrEdges() const;

    /**
     * @return  The number of Nodes in the LineGraph.
     */
    size_t                nrNodes() const;

    /**
     * @return  The number of Nodes in the LineGraph.
     */
    size_t                nrLines() const;

    /**
     * @return  The Boost Graph representation of the Graph.
     */
    const GraphType&      getBoostGraph() const;

    /**
     * @return  The Boost Graph representation of the LineGraph.
     */
    const LineGraphType&  getBoostLineGraph() const;

// INQUIRY
    /**
     * @return  true    If graph has a vertex with given id.
     */
    bool                  hasVertex(VertexIdType vertexId) const;

    /**
     * @return  true    If LineGraph has a node with given id.
     */
    bool                  hasNode(EdgeIdType nodeId) const;

    /** Output information about # vertices, edges, nodes, lines.
     */
    void                  printGraphInformation(std::ostream& os) const;

protected:

private:
// HELPERS

    // buildGraph() ----------------------------------------------------------
    // Used when constructing the internal Boost graph representation
    // from the Topology.
    void                buildGraph();
    void                addTopoVerticesToGraph();
    void                addTopoEdgesToGraph();
    void                addDirectedEdge(EdgeIdType id,
                                        const VertexType& source,
                                        const VertexType& target,
                                        EdgeIdType ix);
    const VertexType&   getGraphVertex(VertexIdType id) const;

    // buidlLineGraph() ------------------------------------------------------
    // Used when transforming the Graph to a LineGraph
    void                buildLineGraph();
    void                addGraphEdgesToLineGraph();
    const NodeType&     getLineGraphNode(NodeIdType id) const;
    void                addGraphEdgeAsLineGraphNode(
                            const EdgeType& rGraphEdge, NodeType& rNode);
    void                connectSourceNodeToTargetNodesViaVertex(
                            const NodeType& rSourceNode,
                            const VertexType& rViaVertex);

    void                printVertices(std::ostream& os) const;
    void                printEdges(std::ostream& os)    const;
    void                printNodes(std::ostream& os)    const;
    void                printLines(std::ostream& os)    const;

// ATTRIBUTES
    GraphType                         mGraph;
    LineGraphType                     mLineGraph;
    TopoVertexIdToGraphVertexMapType  mIdToVertexMap;     // map original id to GraphVertex
    TopoEdgeIdToGraphEdgeMapType      mIdToEdgeMap;       // map original id to GraphEdge
    GraphEdgeIdToNodeMapType          mEdgeIdToNodeMap;   // map GraphEdge.id to LineGraphNode
    const Topology&                   mrTopology;

// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYGRAPH_H_ */
