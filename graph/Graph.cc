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
Graph::Graph(const Topology& rTopology)
    : mGraph(),
      mIdToVertexMap(),
      mIdToEdgeMap(),
      mrTopology(rTopology)
{
    buildGraph();
    buildLineGraph();
}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Graph& rGraph)
{
    os << "Graph: #vertices: " << rGraph.nrVertices()
       << ", #edges: " << rGraph.nrEdges()
       << ", #nodes: " << rGraph.nrNodes()
       << ", #lines: " << rGraph.nrLines()
       << std::endl;


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
    return boost::num_vertices(mLineGraph);
}

size_t
Graph::nrLines() const
{
    return boost::num_edges(mLineGraph);
}

const Graph::GraphType&
Graph::getBGLGraph() const
{
    return mGraph;
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
        const VertexType& s = getGraphVertex(e.source());
        const VertexType& t = getGraphVertex(e.target());

        if(e.roadData().direction == Edge::DirectionType::FROM_TO
        || e.roadData().direction == Edge::DirectionType::BOTH)
        {
            addDirectedEdge(e.id(), s, t, e_ix);
            ++e_ix;
        }

        if(e.roadData().direction == Edge::DirectionType::TO_FROM
        || e.roadData().direction == Edge::DirectionType::BOTH)
        {
            addDirectedEdge(e.id(), t, s, e_ix);
            ++e_ix;
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

const Graph::NodeType&
Graph::getLineGraphNode(NodeIdType id) const
{
    const auto& res = mEdgeIdToNodeMap.find(id);
    if(res == mEdgeIdToNodeMap.end())
    {
        throw GraphException("Graph:getLineGraphNode: Missing node: "
            + std::to_string(id));
    }
    return res->second;
}

void
Graph::addGraphEdgeAsLineGraphNode(const EdgeType& rGraphEdge, NodeType& rNode)
{
//    NodeType node;
    EdgeIdType e_graph_id = boost::get(&GraphEdge::graphEdgeId, mGraph, rGraphEdge);
    EdgeIdType e_topo_id  = boost::get(&GraphEdge::topoEdgeId, mGraph, rGraphEdge);

    if(!hasNode(e_graph_id))
    {
        rNode = boost::add_vertex(mLineGraph);
        mLineGraph[rNode].lgNodeId = e_graph_id;
        mLineGraph[rNode].topoEdgeId = e_topo_id;
        mEdgeIdToNodeMap.insert({e_graph_id, rNode});
    }
    else
    {
        rNode = getLineGraphNode(e_graph_id);
    }
}

void
Graph::connectSourceNodeToTargetNodesViaVertex(
    const NodeType& rSourceNode,
    const VertexType& rViaVertex)
{
    for(auto target_it = boost::out_edges(rViaVertex, mGraph);
        target_it.first != target_it.second;
        ++target_it.first)
    {
        const EdgeType& target = *(target_it.first);
        NodeType target_node;
        addGraphEdgeAsLineGraphNode(target, target_node);

        VertexIdType via_topo_vertex_id =
            boost::get(&GraphVertex::topoVertexId, mGraph, rViaVertex);
        const Vertex& v = mrTopology.getVertex(via_topo_vertex_id);

        bool restricted = false;

        if(v.hasRestrictions())
        {
            // TODO
            // if travel is prohibited:
//            restricted = true;
        }

        if(!restricted)
        {
            NodeIdType source_id =
                boost::get(&LineGraphNode::lgNodeId, mLineGraph, rSourceNode);
            NodeIdType target_id =
                boost::get(&LineGraphNode::lgNodeId, mLineGraph, target_node);

            // add Line between Nodes
            const auto& line_add =
                boost::add_edge(rSourceNode, target_node, mLineGraph);
            if(line_add.second == true)
            {
                const LineType& line  = line_add.first;
                mLineGraph[line].lgSourceNodeId = source_id;
                mLineGraph[line].lgTargetNodeId = target_id;
                mLineGraph[line].topoViaVertexId = via_topo_vertex_id;
                // TODO cost
            }
            else
            {
                throw GraphException(
                    "Graph:connectSourceNodeToTargetNodesViaVertex: source: "
                    + std::to_string(source_id)
                    + ", target: " + std::to_string(target_id));
            }
        }
    }
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
    for(auto n_it = boost::vertices(mLineGraph);
        n_it.first != n_it.second;
        ++n_it.first)
    {
        const NodeType& node = *(n_it.first);
        NodeIdType lg_node_id =
            boost::get(&LineGraphNode::lgNodeId, mLineGraph, node);
        EdgeIdType topo_edge_id  =
            boost::get(&LineGraphNode::topoEdgeId, mLineGraph, node);

        os << "   lg_node_id (graph_edge_id): " << lg_node_id
           << ", topo_edge_id: " << topo_edge_id << std::endl;
    }
}

void
Graph::printLines(std::ostream& os) const
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
