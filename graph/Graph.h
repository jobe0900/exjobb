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
#include <algorithm>
#include <map>
#include <ostream>

#include <iostream>

// PROJECT INCLUDES
//
#include <boost/graph/adjacency_list.hpp>

// LOCAL INCLUDES
//
#include "GraphException.h"
#include "Vertex.h"
#include "Edge.h"
#include "Topology.h"
#include "Restrictions.h"
#include "../config/Configuration.h"
#include "../util/Logging.h"
#include "EdgeRestriction.h"

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
 * A class for a keeping Topology and optional Restrictions and Costs, and
 * from that informations building a Boost graph that is transformed to
 * another Boost graph representing a Line Graph.
 */
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
     * Graph should be based on the supplied topology.
     * @param   rTopology       The topology to use as basis for the graph.
     * @param   rConfig         The configuration used for topology and all.
     */
    Graph(Topology& rTopology, const Configuration& rConfig);

    /** Copy constructor.
     * Disabled.
     */
    Graph(const Graph& from) = delete;

    /** Destructor.
     */
    ~Graph();

// OPERATORS
    /** Output operator to print to a stream.
     */
    friend
    std::ostream&         operator<<(std::ostream& os, const Graph& rGraph);

// OPERATIONS

    /** Flag if restrictions should be used when building the graph or not.
     * @param   shouldUseRestrictions   If true, build graph with restrictions.
     */
    void                  useRestrictions(bool shouldUseRestrictions);

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

    /** Builds graph if necessary before returning.
     * @return  The Boost Graph representation of the Graph.
     * @throws  GraphException if something goes wrong building the graph.
     */
    const GraphType&      getBoostGraph();

    /** Builds graph if necessary before returning.
     * @return  The Boost Graph representation of the LineGraph.
     * @throws  GraphException if something goes wrong building the graph.
     */
    const LineGraphType&  getBoostLineGraph();

// INQUIRY
    /**
     * @return  true    If graph has a vertex with given id.
     */
    bool                  hasVertex(VertexIdType vertexId) const;

    /**
     * @return  true    If LineGraph has a node with given id.
     */
    bool                  hasNode(EdgeIdType nodeId) const;

    /**
     * @return  true    If graph was built with restrictions.
     */
    bool                  isRestricted() const;

    /** Output information about # vertices, edges, nodes, lines.
     */
    void                  printGraphInformation(std::ostream& os) const;

protected:

private:
// HELPERS

    // buildGraph() ----------------------------------------------------------
    // Used when constructing the internal Boost graph representation
    // from the Topology.

    /** Build the graph by adding vertices and edges from the topology. */
    void                buildGraph();

    /** Add the topology vertices to the graph, respecting restrictions.
     * Helper for 'buildGraph()'.
     */
    void                addTopoVerticesToGraph();

    /** Add the topology edges to the graph, respecting restrictions.
     * Helper for 'buildGraph()'.
     */
    void                addTopoEdgesToGraph();

    /** Add a directed edge from source to target.
     * Helper for 'addTopoEdgesToGraph()'.
     * @param   id      The edge's topology id.
     * @param   source  The source vertex.
     * @param   target  The target vertex.
     * @param   e_ix    The running index amongst edges added to graph.
     */
    void                addDirectedEdge(
        EdgeIdType id,
        const VertexType& source,
        const VertexType& target,
        EdgeIdType ix);

    /** Get the graph vertex corresponding to a given id.
     * @param   id      The vertex' topology id.
     * @return  Reference to the Graph vertex corresponding to id.
     * @throw   GraphException if there is no corresponding vertex to id.
     */
    const VertexType&   getGraphVertex(VertexIdType id) const;

    // buidlLineGraph() ------------------------------------------------------
    // Used when transforming the Graph to a LineGraph

    /** Start converting the Graph to a LineGraph.
     */
    void                buildLineGraph();

    /** Add Edges from the graph as Nodes in the Linegraph.
     * Helper for 'buildLineGraph()'
     */
    void                addGraphEdgesToLineGraph();

    /** Actually add a graph edge as a linegraph node, checking if it already
     * exists or not.
     * @param   rGraphEdge  The Edge to add to the LineGraph as Node.
     * @param   rNode       The Node corresponding to the edge returned here.
     */
    void                addGraphEdgeAsLineGraphNode(
        const EdgeType& rGraphEdge,
        NodeType&       rNode);

    /** Get an already existing Node from the LineGraph.
     * @param   id  The Edge id (== the Node id).
     * @param   The LineGraph Node.
     * @throw   GraphException if there is no Node with that id.
     */
    const NodeType&     getLineGraphNode(NodeIdType id) const;

    /** Connect the newly added Node to all Nodes it should be connected to,
     * that is look up which outgoing edges there are from the Edge's (node's)
     * target vertex, and if there are no restrictions: add the Edge as a Node
     * to the LineGraph and add a Line between the Nodes.
     * @param   rSourceNode     The Node to add Lines from.
     * @parma   rViaVertex      Are there any restrictions in the vertex?
     * @throw   GraphException
     */
    void                connectSourceNodeToTargetNodesViaVertex(
        const NodeType& rSourceNode,
        const VertexType& rViaVertex);

    /**
     * @param   edgeId  Id to edge to look up.
     * @return  true if this edge has no exits, meaning it is no use adding it.
     */
    bool                edgeHasNoExit(EdgeIdType edgeId);

    /**
     * @return A vector of all Edges going out from a vertex.
     */
    std::vector<EdgeIdType>
                        getOutEdges(VertexIdType vertexId) const;

    /**
     * @return  A vector of all restricted edges from this Edge.
     */
    std::vector<EdgeIdType>
                        getRestrictedTargets(EdgeIdType edgeId) const;

    /**
     * @return  true if this target edge has restricted access from the source.
     */
    bool                isTargetRestricted(
        const std::vector<EdgeIdType>&  rRestrictedTargets,
        EdgeIdType                      targetId) const;

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
    Topology&                         mrTopology;
    const Configuration&              mrConfiguration;
    mutable boost::log::sources::severity_logger
        <boost::log::trivial::severity_level>
                                      mLog;
    bool                              mUseRestrictions;

// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYGRAPH_H_ */
