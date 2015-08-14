/*
 * Graph.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

#include "Graph.h"  // class implemented

#include <typeinfo>

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Graph::Graph(Topology& rTopology, const Configuration& rConfig)
    : mGraph(),
      mpLineGraph(nullptr),
      mIdToVertexMap(),
      mIdToEdgeMap(),
      mrTopology(rTopology),
      mrConfiguration(rConfig),
      mLog(),
      mUseRestrictions(true)
{
    Logging::initLogging();
    boost::log::add_common_attributes();

    buildGraph();
    buildLineGraph();
}

Graph::~Graph()
{
}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Graph& rGraph)
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
void
Graph::useRestrictions(bool shouldUseRestrictions)
{
    if(shouldUseRestrictions != mUseRestrictions)
    {
        mUseRestrictions = shouldUseRestrictions;

        mGraph.clear();
        if(mpLineGraph != nullptr)
        {
            mpLineGraph->clear();
        }
        mIdToVertexMap.clear();
        mIdToEdgeMap.clear();
        mEdgeIdToNodeMap.clear();

        buildGraph();
        buildLineGraph();
    }
}
//============================= ACESS      ===================================
size_t
Graph::nrVertices() const
{
    return mIdToVertexMap.size();
}

size_t
Graph::nrEdges() const
{
    return mIdToEdgeMap.size();
}

size_t
Graph::nrNodes() const
{
    if(mpLineGraph != nullptr)
    {
        return boost::num_vertices(*mpLineGraph);
    }
    return 0;
}

size_t
Graph::nrLines() const
{
    if(mpLineGraph != nullptr)
    {
        return boost::num_edges(*mpLineGraph);
    }
    return 0;
}

const Graph::GraphType&
Graph::getBoostGraph()
{
    return mGraph;
}

Graph::LineGraphType*
Graph::getBoostLineGraph()
{
    return mpLineGraph;
}

//============================= INQUIRY    ===================================
bool
Graph::hasVertex(VertexIdType vertexId) const
{
    const auto& it = mIdToVertexMap.find(vertexId);
    return (it != mIdToVertexMap.end());
}

bool
Graph::hasNode(EdgeIdType nodeId) const
{
    const auto& it = mEdgeIdToNodeMap.find(nodeId);
    return (it != mEdgeIdToNodeMap.end());
}

bool
Graph::isRestricted() const
{
    return mUseRestrictions;
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
Graph::buildGraph()
{
    addTopoVerticesToGraph();
    addTopoEdgesToGraph();
}

void
Graph::addTopoVerticesToGraph()
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
Graph::addTopoEdgesToGraph()
{
    EdgeIdType e_ix = 0;

    for(const auto& edgepair : mrTopology.mEdgeMap)
    {
        const Edge& e = edgepair.second;

        if(mUseRestrictions && e.isRestricted(mrConfiguration))
        {
            BOOST_LOG_SEV(mLog, boost::log::trivial::info)
                        << "Graph:addTopoEdgeToGraph(): "
                        << "Restricted Source id " << e.id();
            continue;
        }

        const VertexType& s = getGraphVertex(e.source());
        const VertexType& t = getGraphVertex(e.target());

        if(e.roadData().direction == Edge::DirectionType::FROM_TO
        || e.roadData().direction == Edge::DirectionType::BOTH)
        {
            for(size_t lane = 1; lane <= e.roadData().nrLanes; ++lane) {
                addDirectedEdge(e.id(), s, t, e_ix);
                ++e_ix;
            }
        }

        if(e.roadData().direction == Edge::DirectionType::TO_FROM
        || e.roadData().direction == Edge::DirectionType::BOTH)
        {
            for(size_t lane = 1; lane <= e.roadData().nrLanes; ++lane) {
                addDirectedEdge(e.id(), t, s, e_ix);
                ++e_ix;
            }
        }
    }
}

void
Graph::addDirectedEdge(EdgeIdType id,
                       const VertexType& source,
                       const VertexType& target,
                       EdgeIdType e_ix)
{
    const auto& res = boost::add_edge(source, target, mGraph);
    if(res.second == true)
    {
        mIdToEdgeMap.insert({id, res.first});
        mGraph[res.first].graphEdgeId = e_ix;
        mGraph[res.first].topoEdgeId = id;
    }
    else
    {
        throw GraphException("Graph:addDirectedEdge: cannot add edge: "
            + std::to_string(id));
    }
}

const Graph::VertexType&
Graph::getGraphVertex(VertexIdType id) const
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
Graph::buildLineGraph()
{
    delete mpLineGraph;
    mpLineGraph = new LineGraphType();
    addGraphEdgesToLineGraph();
}

void
Graph::addGraphEdgesToLineGraph()
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
Graph::addGraphEdgeAsLineGraphNode(const EdgeType& rGraphEdge, NodeType& rNode)
{
    EdgeIdType e_graph_id = boost::get(&GraphEdge::graphEdgeId, mGraph, rGraphEdge);
    EdgeIdType e_topo_id  = boost::get(&GraphEdge::topoEdgeId, mGraph, rGraphEdge);

    if(!hasNode(e_graph_id))
    {
        rNode = boost::add_vertex(*mpLineGraph);
        (*mpLineGraph)[rNode].graphEdgeId = e_graph_id;
        (*mpLineGraph)[rNode].topoEdgeId = e_topo_id;
        mEdgeIdToNodeMap.insert({e_graph_id, rNode});
    }
    else
    {
        rNode = getLineGraphNode(e_graph_id);
    }
}

const Graph::NodeType&
Graph::getLineGraphNode(NodeIdType id) const
{
    const auto& res = mEdgeIdToNodeMap.find(id);
    if(res == mEdgeIdToNodeMap.end())
    {
        delete mpLineGraph;

        throw GraphException("Graph:getLineGraphNode: Missing node: "
            + std::to_string(id));
    }
    return res->second;
}

void
Graph::connectSourceNodeToTargetNodesViaVertex(
    const NodeType& rSourceNode,
    const VertexType& rViaVertex)
{
    EdgeIdType topo_source_id =
        boost::get(&LineGraphNode::topoEdgeId, *mpLineGraph, rSourceNode);
    NodeIdType source_node_id =
        boost::get(&LineGraphNode::graphEdgeId, *mpLineGraph, rSourceNode);

    if(edgeHasNoExit(topo_source_id))
    {
        return;
    }

    std::vector<EdgeIdType> restricted_targets =
        getRestrictedTargets(topo_source_id);

    VertexIdType via_topo_vertex_id =
        boost::get(&GraphVertex::topoVertexId, mGraph, rViaVertex);

    for(auto target_it = boost::out_edges(rViaVertex, mGraph);
        target_it.first != target_it.second;
        ++target_it.first)
    {
        const EdgeType& target = *(target_it.first);
        EdgeIdType topo_target_id = boost::get(&GraphEdge::topoEdgeId, mGraph, target);

        if(mUseRestrictions && isTargetRestricted(restricted_targets, topo_target_id))
        {
            Edge& s = mrTopology.getEdge(topo_source_id);
            Edge& t = mrTopology.getEdge(topo_target_id);
            BOOST_LOG_SEV(mLog, boost::log::trivial::info)
            << "Graph:connectSourceNodeToTargetNodesViaVertex(): Restricted: "
            << "Source: " << topo_source_id << " (osm: " << s.osmId()
            << ") , Target: " << topo_target_id << " (osm: " << t.osmId() << ")";
        }
        else
        {
            NodeType target_node;
            addGraphEdgeAsLineGraphNode(target, target_node);

            NodeIdType target_node_id =
                boost::get(&LineGraphNode::graphEdgeId, *mpLineGraph, target_node);

            // add Line between Nodes
            const auto& line_add =
                boost::add_edge(rSourceNode, target_node, *mpLineGraph);
            if(line_add.second == true)
            {
                const LineType& line  = line_add.first;
                (*mpLineGraph)[line].lgSourceNodeId = source_node_id;
                (*mpLineGraph)[line].lgTargetNodeId = target_node_id;
                (*mpLineGraph)[line].topoViaVertexId = via_topo_vertex_id;
                (*mpLineGraph)[line].cost =
                    calculateTurnCost(topo_source_id, topo_target_id);
            }
            else
            {
                delete mpLineGraph;
                throw GraphException(
                    "Graph:connectSourceNodeToTargetNodesViaVertex: source: "
                    + std::to_string(source_node_id)
                    + ", target: " + std::to_string(target_node_id));
            }
        }
    }
}

double
Graph::calculateTurnCost(EdgeIdType sourceEdgeId, EdgeIdType targetEdgeId) const
{
    const Edge& source = mrTopology.getEdge(sourceEdgeId);
    const Edge& target = mrTopology.getEdge(targetEdgeId);
    return TurnCostCalculator::getTurnCost(source, target, mrConfiguration);
}

bool
Graph::edgeHasNoExit(EdgeIdType edgeId)
{
    Edge& e = mrTopology.getEdge(edgeId);
    if(e.hasRestrictions() && e.restrictions().hasNoExitRestriction())
    {
        return true;
    }
    return false;
}

std::vector<EdgeIdType>
Graph::getOutEdges(VertexIdType vertexId) const
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
Graph::getRestrictedTargets(EdgeIdType edgeId) const
{
    std::vector<EdgeIdType> restricted_targets;

    // We don't know the direction of the the travel on the edge here so
    // we must find out edges from both source and target vertex.
    std::vector<EdgeIdType> targets;
    VertexIdType source_vertex = mrTopology.getEdge(edgeId).source();
    VertexIdType target_vertex = mrTopology.getEdge(edgeId).target();

    std::vector<EdgeIdType> out_edges = getOutEdges(source_vertex);
    targets.insert(targets.end(), out_edges.begin(), out_edges.end());

    out_edges = getOutEdges(target_vertex);
    targets.insert(targets.end(), out_edges.begin(), out_edges.end());

    for(EdgeIdType e_id : targets)
    {
        if(e_id == edgeId)
        {
            continue;
        }

        Edge& e = mrTopology.getEdge(e_id);

        if(e.isRestricted(mrConfiguration))
        {
            BOOST_LOG_SEV(mLog, boost::log::trivial::info)
                        << "Graph:getRestrictedTargets(): "
                        << "Source id " << edgeId
                        << " has restricted target: " << e_id;
            restricted_targets.push_back(e_id);
        }
    }

    Edge& edge = mrTopology.getEdge(edgeId);

    if(edge.hasRestrictions() && edge.restrictions().hasTurningRestriction())
    {
        BOOST_LOG_SEV(mLog, boost::log::trivial::info)
                    << "Graph:getRestrictedTargets(): "
                    << "Source id " << edgeId << " has TURN restricted targets. ";
        std::vector<EdgeIdType> turn_restricted_targets =
            edge.restrictions().restrictedTargetEdges();
        restricted_targets.insert(restricted_targets.end(),
            turn_restricted_targets.begin(), turn_restricted_targets.end());

    }

    return restricted_targets;
}

bool
Graph::isTargetRestricted(
    const std::vector<EdgeIdType>& rRestrictedTargets,
    EdgeIdType targetId) const
{
    if(rRestrictedTargets.size() > 0)
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
Graph::printGraphInformation(std::ostream& os) const
{
    os << "Graph: #vertices: " << nrVertices()
       << ", #edges: " << nrEdges()
       << ", #nodes: " << nrNodes()
       << ", #lines: " << nrLines()
       << std::endl;
}

void
Graph::printVertices(std::ostream& os) const
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
Graph::printEdges(std::ostream& os) const
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
Graph::printNodes(std::ostream& os) const
{
    for(auto n_it = boost::vertices(*mpLineGraph);
        n_it.first != n_it.second;
        ++n_it.first)
    {
        const NodeType& node = *(n_it.first);
        NodeIdType lg_node_id =
            boost::get(&LineGraphNode::graphEdgeId, *mpLineGraph, node);
        EdgeIdType topo_edge_id  =
            boost::get(&LineGraphNode::graphEdgeId, *mpLineGraph, node);

        os << "   lg_node_id (graph_edge_id): " << lg_node_id
           << ", topo_edge_id: " << topo_edge_id << std::endl;
    }
}

void
Graph::printLines(std::ostream& os) const
{
    for(auto line_it = boost::edges(*mpLineGraph);
        line_it.first != line_it.second;
        ++line_it.first)
    {
        const LineType& line = *(line_it.first);
        NodeIdType lg_source_id =
            boost::get(&LineGraphLine::lgSourceNodeId, *mpLineGraph, line);
        NodeIdType lg_target_id =
            boost::get(&LineGraphLine::lgTargetNodeId, *mpLineGraph, line);
        VertexIdType topo_via_vertex_id =
            boost::get(&LineGraphLine::topoViaVertexId, *mpLineGraph, line);

        os << "   lg_source_id: " << lg_source_id
           << ", lg_target_id: " << lg_target_id
           << ", topo_via_vertex_id: " << topo_via_vertex_id << std::endl;
    }
}
