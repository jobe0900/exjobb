/**  GraphBuilder.
 *
 * #include "GraphBuilder.h"
 *
 * @author  Jonas Bergman
 */

#ifndef GRAPH_GRAPHBUILDER_H_
#define GRAPH_GRAPHBUILDER_H_

// SYSTEM INCLUDES
//
#include <algorithm>
#include <map>
#include <ostream>

// PROJECT INCLUDES
//
#include <boost/graph/adjacency_list.hpp>

// LOCAL INCLUDES
//
#include "Edge.h"
#include "EdgeRestriction.h"
#include "GraphException.h"
#include "Topology.h"
#include "TurnCostCalculator.h"
#include "Vertex.h"
#include "../config/Configuration.h"
#include "../util/Logging.h"

// FORWARD REFERENCES
//

// TYPES
typedef EdgeIdType  NodeIdType;
typedef EdgeIdType  LineIdType;

/**
 * Map the GraphEdges to the original Edge id in the Topology.
 * Also indicate if the GraphEdge is the same or opposite direction to
 * the graph in the topology.
 */
struct GraphEdge
{
    EdgeIdType      graphEdgeId;
    EdgeIdType      topoEdgeId;
    bool            oppositeDirection {false};
};

/**
 * A Node in the LineGraph corresponds directly to an Edge in the original
 * GraphBuilder and topology. It is connected to another Node (Edge) if both the
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

/** The 'normal' vertex based graph type. */
typedef boost::adjacency_list
    < boost::listS, boost::vecS, boost::directedS,
    GraphVertex, GraphEdge >                                   GraphType;
/** The edge based graph type. */
typedef boost::adjacency_list
    < boost::listS, boost::vecS, boost::directedS,
    LineGraphNode, LineGraphLine >                             LineGraphType;

/** A vertex in the normal graph. */
typedef boost::graph_traits<GraphType>::vertex_descriptor      VertexType;
/** An edge in the normal graph. */
typedef boost::graph_traits<GraphType>::edge_descriptor        EdgeType;

/** A node in the line graph. */
typedef boost::graph_traits<LineGraphType>::vertex_descriptor  NodeType;
/** An edge in the line graph. */
typedef boost::graph_traits<LineGraphType>::edge_descriptor    LineType;

/** Mapping of a topology vertex id and graph vertex object. */
typedef std::map<VertexIdType, VertexType>   TopoVertexIdToGraphVertexMapType;
/** Mapping of a topology edge id and graph edge object. */
typedef std::multimap<EdgeIdType, EdgeType>  TopoEdgeIdToGraphEdgeMapType;
/** Mapping of a graph edge id and linegraph node object. */
typedef std::map<EdgeIdType, NodeType>       GraphEdgeIdToNodeMapType;


/**
 * A class for building (Boost) Graph and LineGraph from a Topology and
 * a Configuration with optional Restrictions and Costs applied.
 */
class GraphBuilder
{
public:
// LIFECYCLE
    /** Constructor.
     * Disabled.
     */
    GraphBuilder() = delete;

    /** Constructor.
     * GraphBuilder should be based on the supplied topology.
     * @param   rTopology       The topology to use as basis for the graph.
     * @param   rConfig         The configuration used for topology and all.
     * @param   useRestrictions If the graph should be built with restrictions or not.
     */
    GraphBuilder(
        Topology& rTopology,
        const Configuration& rConfig,
        bool useRestrictions = true);

    /** Copy constructor.
     * Disabled.
     */
    GraphBuilder(const GraphBuilder& from) = delete;

    /** Destructor.
     */
    ~GraphBuilder();

// OPERATORS
    /** Output operator to print to a stream.
     */
    friend
    std::ostream&       operator<<(std::ostream& os, const GraphBuilder& rGraph);

// OPERATIONS
// ACCESS
    /**
     * @return  The number of Vertices in the Graph.
     */
    size_t              nrVertices() const;

    /**
     * @return  The number of Edges in the Graph.
     */
    size_t              nrEdges() const;

    /**
     * @return  The number of Nodes in the LineGraph.
     */
    size_t              nrNodes() const;

    /**
     * @return  The number of Nodes in the LineGraph.
     */
    size_t              nrLines() const;

    /** Builds graph if necessary before returning.
     * @return  The Boost Graph representation of the Graph.
     * @throws  GraphException if something goes wrong building the graph.
     */
    const GraphType&    getBoostGraph();

    /** Get a reference to the line graph.
     * @return  The Boost Graph representation of the LineGraph.
     * @throws  GraphException if something goes wrong building the graph.
     */
    LineGraphType&       getBoostLineGraph();
    const LineGraphType& getBoostLineGraph() const;

    /** Get access to the topology that is the base for the graph.
     * @return  The Topology
     */
    const Topology&     getTopology() const;

// INQUIRY
    /**
     * @return  true    If graph has a vertex with given id.
     */
    bool                hasVertex(VertexIdType vertexId) const;

    /**
     * @return  true    If LineGraph has a node with given id.
     */
    bool                hasNode(EdgeIdType nodeId) const;

    /** Get an already existing Node from the LineGraph.
     * @param   id  The Edge id (== the Node id).
     * @param   The LineGraph Node.
     * @throw   GraphException if there is no Node with that id.
     */
    const NodeType&     getLineGraphNode(NodeIdType id) const;

    /**
     * @return  true    If graph was built with restrictions.
     */
    bool                isRestricted() const;

    /** Output information about # vertices, edges, nodes, lines.
     */
    void                printGraphInformation(std::ostream& os) const;

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

    /** Check if an edge is restricted
     * @param   rEdge   Reference to edge
     * @return  bool
     */
    bool                isEdgeRestricted(const Edge& rEdge) const;

    /** Add the correct number of directed edges from the topo Edge.
     * @param   rEdge        The topological graph data
     * @param   rNewEdgeId   The running id for the graph's directed edges.
     */
    void                addDirectedGraphEdges(
                            const Edge& rEdge,
                            EdgeIdType& rNewEdgeId);

    /** Add a directed edge from source to target.
     * Helper for 'addTopoEdgesToGraph()'.
     * @param   id      The edge's topology id.
     * @param   source  The source vertex.
     * @param   target  The target vertex.
     * @param   e_ix    The running index amongst edges added to graph.
     * @param   oppositeDirection
     *                  If the directed edge runs opposite of the original
     *                  edge direction as specified in the topology.
     */
    void                addDirectedEdge(
                            EdgeIdType id,
                            const VertexType& source,
                            const VertexType& target,
                            EdgeIdType ix,
                            bool  oppositeDirection);

    /** Get the graph vertex corresponding to a given id.
     * @param   id      The vertex' topology id.
     * @return  Reference to the Graph vertex corresponding to id.
     * @throw   GraphException if there is no corresponding vertex to id.
     */
    const VertexType&   getGraphVertex(VertexIdType id) const;

    // buidlLineGraph() ------------------------------------------------------
    // Used when transforming the Graph to a LineGraph

    /** Start converting the GraphBuilder to a LineGraph.
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

    /** Extract LineGraphNode data from the LineGraph.
     * @param   rNode     The descriptor in the LineGraph
     * @return  a LineGraphNode
     */
    LineGraphNode       getLineGraphNodeData(const NodeType& rNode) const;

    /** Add a line in the LineGraph, connecting the source and target nodes.
     * @param   rSourceNode
     * @param   rTargetNode
     * @return  the added line
     * @throw   GraphException
     */
    LineType            addLineGraphLine(
                            const NodeType& rSourceNode,
                            const NodeType& rTargetNode);

    /** Add meta data ids for source, target and vertex to the newly added Line.
     * @param   rLine
     * @param   sourceId
     * @param   targetId
     * @param   viaVertexId
     */
    void                addLineMetaIds(
                            const LineType& rLine,
                            EdgeIdType sourceId,
                            EdgeIdType targetId,
                            VertexIdType viaVertexId);

    /** Add the meta information about the cost to the new line.
     * @param   rLine
     * @param   rSourceEdge     The Source Edge
     * @param   targetId        The id of the target edge in topology
     */
    void                addLineMetaCost(
                            const LineType& rLine,
                            const Edge&     rSourceEdge,
                            EdgeIdType      targetId);

    /** Calculate the cost for making a turn from source edge to target.
     * Helper to `connectSourceNodeToTargetNodesViaVertex()`.
     * @param   sourceEdgeId    The edge (and node) id of the source.
     * @param   targetEdgeId    The edge (and node) id of the target.
     */
    double              calculateTurnCost(
                            EdgeIdType sourceEdgeId,
                            EdgeIdType targetEdgeId) const;

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
     * @param   rSourceNode     The LineGraph Node
     * @return  A vector of all restricted edges from this Edge.
     */
    std::vector<EdgeIdType>
                        getRestrictedTargets(
                            const LineGraphNode& rSourceNode) const;

    /** Look through the targets from a source to find which are restricted
     * and add them to a collection of restricted.
     * @param   rSsourceEdge        The source edge.
     * @param   rTargets            Targets from that source.
     * @param   rRestrictedTargets  A collection to build up.
     */
    void                findRestrictedTargets(
                            const Edge&                     rSourceEdge,
                            const std::vector<EdgeIdType>&  rTargets,
                            std::vector<EdgeIdType>&        rRestrictedTargets) const;

    /** Add the turning restricted targets to the other restricted targets.
     * @param   rSourceEdge         The source edge.
     * @param   rRestrictedTargets  The collection of restricted targets.
     */
    void                addTurningRestrictedTargets(
                            const Edge&               rSource,
                            std::vector<EdgeIdType>&  rRestrictedTargets) const;

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

#endif /* GRAPH_GRAPHBUILDER_H_ */
