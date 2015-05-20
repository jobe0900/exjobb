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
       << ", #edges: " << rGraph.nrEdges() << std::endl
       << std::endl;

    rGraph.printVertices(os);
    os << std::endl;
    rGraph.printEdges(os);

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

const Graph::GraphType&
Graph::getGraph() const
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
        const auto& e = *(e_it.first);
        EdgeIdType e_graph_id = boost::get(&GraphEdge::graphEdgeId, mGraph, e);
        EdgeIdType e_topo_id  = boost::get(&GraphEdge::topoEdgeId, mGraph, e);

        NodeType node = boost::add_vertex(mLineGraph);
        mLineGraph[node].lineGraphNodeId = e_graph_id;
        mLineGraph[node].topoEdgeId = e_topo_id;

        // look up target vertex.
        VertexType via_vertex = boost::target(e, mGraph);

        // look at outgoing edges from vertex.
        for(auto out_it = boost::out_edges(via_vertex, mGraph);
            out_it.first != out_it.second;
            ++out_it.first)
        {
            const auto& out = *(out_it.first);
            EdgeIdType out_graph_id = boost::get(&GraphEdge::graphEdgeId, mGraph, out);
            EdgeIdType out_topo_id  = boost::get(&GraphEdge::topoEdgeId, mGraph, out);

            if(!hasNode(out_graph_id))
            {
                NodeType out_node = boost::add_vertex(mLineGraph);
                mLineGraph[out_node].lineGraphNodeId = out_graph_id;
                mLineGraph[out_node].topoEdgeId = out_topo_id;
                mEdgeIdToNodeMap.insert({out_graph_id, out_node});

                // TODO look up restrictions
                VertexIdType via_topo_id =
                    boost::get(&GraphVertex::topoVertexId, mGraph, via_vertex);
                const Vertex& v = mrTopology.getVertex(via_topo_id);
                if(!v.hasRestrictions()) {
                    // add Line between Nodes
                    LineType line = boost::add_edge(node, out_node, mLineGraph);
                }
//                else //look up restrictions for these edges/via node.
//                {
//
//                }
            }
        }

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
Graph::printVertices(std::ostream& os) const
{
    for(auto v_it = boost::vertices(mGraph);
        v_it.first != v_it.second;
        ++v_it.first)
    {
        const auto& v = *v_it.first;
        VertexIdType v_graph_id = boost::get(&GraphVertex::graphVertexId, mGraph, v);
        VertexIdType v_topo_id  = boost::get(&GraphVertex::topoVertexId, mGraph, v);
        const Vertex& vertex = mrTopology.getVertex(v_topo_id);

        os << "   v_graph_id: " << v_graph_id
           << ", v_topo_id: " << v_topo_id
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
        const auto& e = *(e_it.first);
        EdgeIdType e_graph_id = boost::get(&GraphEdge::graphEdgeId, mGraph, e);
        EdgeIdType e_topo_id  = boost::get(&GraphEdge::topoEdgeId, mGraph, e);
        const Edge& edge = mrTopology.getEdge(e_topo_id);

        os << "   e_graph_id: " << e_graph_id
           << ", e_topo_id: " << e_topo_id
           << "\n      e: " << e
           << "  " << edge << std::endl;
    }
}
