/*
 * GraphBuilder.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

#include "GraphBuilder.h"  // class implemented

#include <typeinfo>

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
GraphBuilder::GraphBuilder(
    Topology& rTopology,
    const Configuration& rConfig,
    bool useRestrictions)
    : mGraph(),
      mLineGraph(),
      mIdToVertexMap(),
      mIdToEdgeMap(),
      mrTopology(rTopology),
      mrConfiguration(rConfig),
      mLog(),
      mUseRestrictions(useRestrictions)
{
    Logging::initLogging();
    boost::log::add_common_attributes();

    buildGraph();
    buildLineGraph();
}

GraphBuilder::~GraphBuilder()
{
}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const GraphBuilder& rGraph)
{
    rGraph.printGraphInformation(os);

    os << std::endl << "Vertices: " << std::endl;
    rGraph.printVertices(os);

    os << std::endl << "Edges: " << std::endl;
    rGraph.printEdges(os);

    os << std::endl << "Nodes: " << std::endl;
    rGraph.printNodes(os);

    os << std::endl << "Lines: " << std::endl;
    rGraph.printLines(os);

    return os;
}
//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
size_t
GraphBuilder::nrVertices() const
{
    return mIdToVertexMap.size();
}

size_t
GraphBuilder::nrEdges() const
{
    return mIdToEdgeMap.size();
}

size_t
GraphBuilder::nrNodes() const
{
    return boost::num_vertices(mLineGraph);
}

size_t
GraphBuilder::nrLines() const
{
    return boost::num_edges(mLineGraph);
}

const GraphType&
GraphBuilder::getBoostGraph()
{
    return mGraph;
}

LineGraphType&
GraphBuilder::getBoostLineGraph()
{
    return mLineGraph;
}

const LineGraphType&
GraphBuilder::getBoostLineGraph() const
{
    return mLineGraph;
}

const Topology&
GraphBuilder::getTopology() const
{
    return mrTopology;
}

//============================= INQUIRY    ===================================
bool
GraphBuilder::hasVertex(VertexIdType vertexId) const
{
    const auto& it = mIdToVertexMap.find(vertexId);
    return (it != mIdToVertexMap.end());
}

bool
GraphBuilder::hasNode(EdgeIdType nodeId) const
{
    const auto& it = mEdgeIdToNodeMap.find(nodeId);
    return (it != mEdgeIdToNodeMap.end());
}

const NodeType&
GraphBuilder::getLineGraphNode(NodeIdType id) const
{
    const auto& res = mEdgeIdToNodeMap.find(id);
    if(res == mEdgeIdToNodeMap.end())
    {
        throw GraphException("Graph:getLineGraphNode: Missing node: "
            + std::to_string(id));
    }
    return res->second;
}

bool
GraphBuilder::isRestricted() const
{
    return mUseRestrictions;
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
GraphBuilder::buildGraph()
{
    addTopoVerticesToGraph();
    addTopoEdgesToGraph();
}

void
GraphBuilder::addTopoVerticesToGraph()
{
    VertexIdType v_ix = 0;
    for(const auto& vertexpair : mrTopology.mVertexMap)
    {
        VertexType v = boost::add_vertex(mGraph);
        mIdToVertexMap.insert({vertexpair.second.id(), v});
        mGraph[v].graphVertexId = v_ix;
        mGraph[v].topoVertexId = vertexpair.second.id();
        ++v_ix;
    }
}

void
GraphBuilder::addTopoEdgesToGraph()
{
    EdgeIdType e_ix = 0;

    for(const auto& edgepair : mrTopology.mEdgeMap)
    {
        const Edge& e = edgepair.second;

        if(isEdgeRestricted(e))
        {
            continue;
        }

        addDirectedGraphEdges(e, e_ix);
    }
}


bool
GraphBuilder::isEdgeRestricted(const Edge& rEdge) const
{
    if(mUseRestrictions && rEdge.isRestricted(mrConfiguration))
    {
        BOOST_LOG_SEV(mLog, boost::log::trivial::info)
                            << "Graph:addTopoEdgeToGraph(): "
                            << "Restricted Edge id " << rEdge.id();
        return true;
    }
    return false;
}

void
GraphBuilder::addDirectedGraphEdges(const Edge& rEdge, EdgeIdType& rNewEdgeId)
{
    const VertexType& s = getGraphVertex(rEdge.source());
    const VertexType& t = getGraphVertex(rEdge.target());

    // add all lanes in forward direction
    if(rEdge.roadData().direction == Edge::DirectionType::FROM_TO
        || rEdge.roadData().direction == Edge::DirectionType::BOTH)
    {
        for(size_t lane = 1; lane <= rEdge.roadData().nrLanes; ++lane) {
            addDirectedEdge(rEdge.id(), s, t, rNewEdgeId, false);
            ++rNewEdgeId;
        }
    }

    // add all lanes in backward direction
    if(rEdge.roadData().direction == Edge::DirectionType::TO_FROM
        || rEdge.roadData().direction == Edge::DirectionType::BOTH)
    {
        for(size_t lane = 1; lane <= rEdge.roadData().nrLanes; ++lane) {
            addDirectedEdge(rEdge.id(), t, s, rNewEdgeId, true);
            ++rNewEdgeId;
        }
    }
}

void
GraphBuilder::addDirectedEdge(
    EdgeIdType          id,
    const VertexType&   source,
    const VertexType&   target,
    EdgeIdType          e_ix,
    bool                oppositeDirection)
{
    const auto& res = boost::add_edge(source, target, mGraph);
    if(res.second == true)
    {
        mIdToEdgeMap.insert({id, res.first});
        mGraph[res.first].graphEdgeId = e_ix;
        mGraph[res.first].topoEdgeId = id;
        mGraph[res.first].oppositeDirection = oppositeDirection;
    }
    else
    {
        throw GraphException("Graph:addDirectedEdge: cannot add edge: "
            + std::to_string(id));
    }
}

const VertexType&
GraphBuilder::getGraphVertex(VertexIdType id) const
{
    const auto& res = mIdToVertexMap.find(id);
    if(res == mIdToVertexMap.end())
    {
        throw GraphException("Graph:getGraphVertex: Missing vertex: "
            + std::to_string(id));
    }
    return res->second;
}

void
GraphBuilder::buildLineGraph()
{
    mLineGraph.clear();
    addGraphEdgesToLineGraph();
}

void
GraphBuilder::addGraphEdgesToLineGraph()
{
    // iterate through edges: add as Node.
    for(auto e_it = boost::edges(mGraph);
        e_it.first != e_it.second;
        ++e_it.first)
    {
        const EdgeType& edge = *(e_it.first);

        NodeType node;
        addGraphEdgeAsLineGraphNode(edge, node);

        // look up target vertex.
        VertexType via_vertex = boost::target(edge, mGraph);

        // connect all possible travels from 'edge' via the vertex
        connectSourceNodeToTargetNodesViaVertex(node, via_vertex);
    }
}

void
GraphBuilder::addGraphEdgeAsLineGraphNode(const EdgeType& rGraphEdge, NodeType& rNode)
{
    EdgeIdType e_graph_id = boost::get(&GraphEdge::graphEdgeId, mGraph, rGraphEdge);
    EdgeIdType e_topo_id  = boost::get(&GraphEdge::topoEdgeId, mGraph, rGraphEdge);

    if(!hasNode(e_graph_id))
    {
        rNode = boost::add_vertex(mLineGraph);
        mLineGraph[rNode].graphEdgeId = e_graph_id;
        mLineGraph[rNode].topoEdgeId = e_topo_id;
        mEdgeIdToNodeMap.insert({e_graph_id, rNode});
    }
    else
    {
        rNode = getLineGraphNode(e_graph_id);
    }
}


void
GraphBuilder::connectSourceNodeToTargetNodesViaVertex(
    const NodeType& rSourceNode,
    const VertexType& rViaVertex)
{
    // SOURCE
    LineGraphNode source_node = getLineGraphNodeData(rSourceNode);

    if(edgeHasNoExit(source_node.topoEdgeId))
    {
        return;
    }

    // get the edge corresponding to the node
    Edge& source_edge = mrTopology.getEdge(source_node.topoEdgeId);

    // VIA
    VertexIdType via_topo_vertex_id =
        boost::get(&GraphVertex::topoVertexId, mGraph, rViaVertex);

    // TARGET
    // get targets that are restricted
    std::vector<EdgeIdType> restricted_targets = getRestrictedTargets(source_node);

    // look at all out edges from the via-vertex
    for(auto target_it = boost::out_edges(rViaVertex, mGraph);
        target_it.first != target_it.second;
        ++target_it.first)
    {
        const EdgeType& target = *(target_it.first);
        EdgeIdType target_topo_id = boost::get(&GraphEdge::topoEdgeId, mGraph, target);

        if(!isTargetRestricted(restricted_targets, target_topo_id))
        {
            // add nodes to LineGraph
            NodeType target_node;
            addGraphEdgeAsLineGraphNode(target, target_node);

            NodeIdType target_edge_id =
                boost::get(&LineGraphNode::graphEdgeId, mLineGraph, target_node);

            // add Line between Nodes to the LineGraph
            try
            {
                LineType line = addLineGraphLine(rSourceNode, target_node);
                addLineMetaIds(
                    line,
                    source_node.graphEdgeId,
                    target_edge_id,
                    via_topo_vertex_id);
                addLineMetaCost(line, source_edge, target_topo_id);
            }
            catch (GraphException& ge)
            {
                throw GraphException(
                    "Graph:connectSourceNodeToTargetNodesViaVertex: source: "
                    + std::to_string(source_node.graphEdgeId)
                    + ", target: " + std::to_string(target_edge_id));
            }
        }
        else // log restricted target
        {
            Edge& s = mrTopology.getEdge(source_node.topoEdgeId);
            Edge& t = mrTopology.getEdge(target_topo_id);
            BOOST_LOG_SEV(mLog, boost::log::trivial::info)
            << "Graph:connectSourceNodeToTargetNodesViaVertex(): Restricted: "
            << "Source: " << source_node.topoEdgeId << " (osm: " << s.osmId()
            << ") , Target: " << target_topo_id << " (osm: " << t.osmId() << ")";
        }
    }
}

LineGraphNode
GraphBuilder::getLineGraphNodeData(const NodeType& rNode) const
{
    LineGraphNode node;
    node.topoEdgeId = boost::get(&LineGraphNode::topoEdgeId, mLineGraph, rNode);
    node.graphEdgeId = boost::get(&LineGraphNode::graphEdgeId, mLineGraph, rNode);
    node.oppositeDirection =
        boost::get(&LineGraphNode::oppositeDirection, mLineGraph, rNode);

    return node;
}

LineType
GraphBuilder::addLineGraphLine(const NodeType& rSourceNode, const NodeType& rTargetNode)
{
    const auto& line_add =
        boost::add_edge(rSourceNode, rTargetNode, mLineGraph);
    if(line_add.second == true)
    {
        return line_add.first;
    }
    else // could not add the line to the linegraph
    {
        throw GraphException("GraphBuilder:addLineGraphLine");
    }
}

void
GraphBuilder::addLineMetaIds(
    const LineType& rLine,
    EdgeIdType      sourceId,
    EdgeIdType      targetId,
    VertexIdType    viaVertexId)
{
    mLineGraph[rLine].lgSourceNodeId = sourceId;
    mLineGraph[rLine].lgTargetNodeId = targetId;
    mLineGraph[rLine].topoViaVertexId = viaVertexId;
}

void
GraphBuilder::addLineMetaCost(
    const LineType& rLine,
    const Edge&     rSourceEdge,
    EdgeIdType      targetId)
{
    mLineGraph[rLine].cost =
        rSourceEdge.cost() +
        calculateTurnCost(rSourceEdge.id(), targetId);
}

double
GraphBuilder::calculateTurnCost(EdgeIdType sourceEdgeId, EdgeIdType targetEdgeId) const
{
    const Edge& source = mrTopology.getEdge(sourceEdgeId);
    const Edge& target = mrTopology.getEdge(targetEdgeId);
    return TurnCostCalculator::getTurnCost(source, target, mrConfiguration);
}

bool
GraphBuilder::edgeHasNoExit(EdgeIdType edgeId)
{
    Edge& e = mrTopology.getEdge(edgeId);
    if(e.hasRestrictions() && e.restrictions().hasNoExitRestriction())
    {
        return true;
    }
    return false;
}

std::vector<EdgeIdType>
GraphBuilder::getOutEdges(VertexIdType vertexId) const
{
    std::vector<EdgeIdType> out_edges;
    VertexType graphVertex = getGraphVertex(vertexId);
    auto edge_iterators = boost::out_edges(graphVertex, mGraph);
    while(edge_iterators.first != edge_iterators.second) {
        const EdgeType& e = *(edge_iterators.first);
        EdgeIdType edgeId = boost::get(&GraphEdge::topoEdgeId, mGraph, e);
        out_edges.push_back(edgeId);
        ++edge_iterators.first;
    }
    return out_edges;
}

std::vector<EdgeIdType>
GraphBuilder::getRestrictedTargets(const LineGraphNode& rSourceNode) const
{
    std::vector<EdgeIdType> restricted_targets;

    // Find all out edges from the target vertex of the edge,
    // which depends on if the edge is the opposite direction of the topo edge.
    Edge& sourceEdge = mrTopology.getEdge(rSourceNode.topoEdgeId);

    VertexIdType target_vertex =
        rSourceNode.oppositeDirection ? sourceEdge.source() : sourceEdge.target();

    std::vector<EdgeIdType> out_edges = getOutEdges(target_vertex);
    std::vector<EdgeIdType> targets;
    targets.insert(targets.end(), out_edges.begin(), out_edges.end());

    // build map of restricted targets
    findRestrictedTargets(sourceEdge, targets, restricted_targets);

    return restricted_targets;
}

void
GraphBuilder::findRestrictedTargets(
    const Edge&                     rSourceEdge,
    const std::vector<EdgeIdType>&  rTargets,
    std::vector<EdgeIdType>&        rRestrictedTargets) const
{
    for(EdgeIdType e_id : rTargets)
    {
        // don't add self to target
        if(e_id == rSourceEdge.id())
        {
            continue;
        }

        Edge& e = mrTopology.getEdge(e_id);

        if(e.isRestricted(mrConfiguration))
        {
            BOOST_LOG_SEV(mLog, boost::log::trivial::info)
                        << "Graph:getRestrictedTargets(): "
                        << "Source id " << rSourceEdge.id()
                        << " has restricted target: " << e_id;
            rRestrictedTargets.push_back(e_id);
        }
    }

    addTurningRestrictedTargets(rSourceEdge, rRestrictedTargets);
}

void
GraphBuilder::addTurningRestrictedTargets(
    const Edge& rSourceEdge,
    std::vector<EdgeIdType>& rRestrictedTargets) const
{
    if(rSourceEdge.hasRestrictions() &&
        rSourceEdge.restrictions().hasTurningRestriction())
    {
        BOOST_LOG_SEV(mLog, boost::log::trivial::info)
                    << "Graph:getRestrictedTargets(): "
                    << "Source id " << rSourceEdge.id()
                    << " has TURN restricted targets. ";
        std::vector<EdgeIdType> turn_restricted_targets =
            rSourceEdge.restrictions().restrictedTargetEdges();
        rRestrictedTargets.insert(rRestrictedTargets.end(),
            turn_restricted_targets.begin(), turn_restricted_targets.end());
    }
}

bool
GraphBuilder::isTargetRestricted(
    const std::vector<EdgeIdType>& rRestrictedTargets,
    EdgeIdType targetId) const
{
    if(mUseRestrictions && rRestrictedTargets.size() > 0)
    {
        const auto& restr_it = std::find(
            rRestrictedTargets.begin(),
            rRestrictedTargets.end(),
            targetId);
        if(restr_it != rRestrictedTargets.end())
        {
            BOOST_LOG_SEV(mLog, boost::log::trivial::info)
                << "Graph:isTargetRestricted(): "
                << "Restricted target id " << targetId;
            return true;
        }
    }
    return false;
}

void
GraphBuilder::printGraphInformation(std::ostream& os) const
{
    os << "Graph: #vertices: " << nrVertices()
       << ", #edges: " << nrEdges()
       << ", #nodes: " << nrNodes()
       << ", #lines: " << nrLines()
       << std::endl;
}

void
GraphBuilder::printVertices(std::ostream& os) const
{
    for(auto v_it = boost::vertices(mGraph);
        v_it.first != v_it.second;
        ++v_it.first)
    {
        const VertexType& v = *v_it.first;
        VertexIdType graph_vertex_id = boost::get(&GraphVertex::graphVertexId, mGraph, v);
        VertexIdType topo_vertex_id  = boost::get(&GraphVertex::topoVertexId, mGraph, v);
        const Vertex& vertex = mrTopology.getVertex(topo_vertex_id);

        os << "   graph_vertex_id: " << graph_vertex_id
           << ", topo_vertex_id: " << topo_vertex_id
           << "\n      v: " << v
           << "  " << vertex << std::endl;
    }
}

void
GraphBuilder::printEdges(std::ostream& os) const
{
    for(auto e_it = boost::edges(mGraph);
        e_it.first != e_it.second;
        ++e_it.first)
    {
        const EdgeType& e = *(e_it.first);
        EdgeIdType graph_edge_id = boost::get(&GraphEdge::graphEdgeId, mGraph, e);
        EdgeIdType topo_edge_id  = boost::get(&GraphEdge::topoEdgeId, mGraph, e);
        const Edge& edge = mrTopology.getEdge(topo_edge_id);

        os << "   graph_edge_id: " << graph_edge_id
           << ", e_topo_id: " << topo_edge_id
           << "\n      e: " << e
           << "  " << edge << std::endl;
    }
}

void
GraphBuilder::printNodes(std::ostream& os) const
{
    for(auto n_it = boost::vertices(mLineGraph);
        n_it.first != n_it.second;
        ++n_it.first)
    {
        const NodeType& node = *(n_it.first);
        NodeIdType lg_node_id =
            boost::get(&LineGraphNode::graphEdgeId, mLineGraph, node);
        EdgeIdType topo_edge_id  =
            boost::get(&LineGraphNode::graphEdgeId, mLineGraph, node);

        os << "   lg_node_id (graph_edge_id): " << lg_node_id
           << ", topo_edge_id: " << topo_edge_id << std::endl;
    }
}

void
GraphBuilder::printLines(std::ostream& os) const
{
    for(auto line_it = boost::edges(mLineGraph);
        line_it.first != line_it.second;
        ++line_it.first)
    {
        const LineType& line = *(line_it.first);
        NodeIdType lg_source_id =
            boost::get(&LineGraphLine::lgSourceNodeId, mLineGraph, line);
        NodeIdType lg_target_id =
            boost::get(&LineGraphLine::lgTargetNodeId, mLineGraph, line);
        VertexIdType topo_via_vertex_id =
            boost::get(&LineGraphLine::topoViaVertexId, mLineGraph, line);

        os << "   lg_source_id: " << lg_source_id
           << ", lg_target_id: " << lg_target_id
           << ", topo_via_vertex_id: " << topo_via_vertex_id << std::endl;
    }
}
