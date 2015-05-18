/*
 * Graph.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

#include "Graph.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Graph::Graph(const Topology& rTopology)
    : mGraph(),
      mIdToVertexMap(),
      mIdToEdgeMap(),
      mrTopology(rTopology)
{
    addTopoVerticesToGraph();
    addTopoEdgesToGraph();
}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Graph& rGraph)
{
    os << "Graph: #vertices: " << rGraph.nrVertices()
       << ", #edges: " << rGraph.nrEdges() << std::endl
       << std::endl;

    for(auto v_it = boost::vertices(rGraph.mGraph);
        v_it.first != v_it.second;
        ++v_it.first)
    {
        const auto& v = *v_it.first;
        VertexIdType v_graph_id = boost::get(&GraphVertex::graphVertexId, rGraph.mGraph, v);
        VertexIdType v_topo_id  = boost::get(&GraphVertex::topoVertexId, rGraph.mGraph, v);
//        VertexIdType v_id = rGraph.mVertexToIdMap.at(v);
        const Vertex& vertex = rGraph.mrTopology.getVertex(v_topo_id);
        os << "   v_graph_id: " << v_graph_id
           << ", v_topo_id: " << v_topo_id
           << "\n      v: " << v
           << "  " << vertex << std::endl;
    }

    os << std::endl;

    for(auto e_it = boost::edges(rGraph.mGraph);
        e_it.first != e_it.second;
        ++e_it.first)
    {
        const auto& e = *(e_it.first);
        EdgeIdType e_graph_id = boost::get(&GraphEdge::graphEdgeId, rGraph.mGraph, e);
        EdgeIdType e_topo_id  = boost::get(&GraphEdge::topoEdgeId, rGraph.mGraph, e);
//        EdgeIdType e_id = rGraph.mEdgeToIdMap.at(e);
        const Edge& edge = rGraph.mrTopology.getEdge(e_topo_id);
        os << "   e_graph_id: " << e_graph_id
           << ", e_topo_id: " << e_topo_id
           << "\n      e: " << e
           << "  " << edge << std::endl;
    }

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
Graph::getRepresentation() const
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

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
Graph::addTopoVerticesToGraph()
{
    VertexIdType ix = 0;
    for(const auto& vertexpair : mrTopology.vertexMap)
    {
        VertexType v = boost::add_vertex(mGraph);
        mIdToVertexMap.insert({vertexpair.second.id(), v});
        mGraph[v].graphVertexId = ix;
        mGraph[v].topoVertexId = vertexpair.second.id();
        ++ix;
//        mVertexToIdMap.insert({v, vertexpair.second.id()});
    }
}

void
Graph::addTopoEdgesToGraph()
{
    EdgeIdType ix = 0;
    for(const auto& edgepair : mrTopology.edgeMap)
    {
        const VertexType& s = getGraphVertex(edgepair.second.source());
        const VertexType& t = getGraphVertex(edgepair.second.target());

        if(edgepair.second.direction() == Edge::Direction::FROM_TO
        || edgepair.second.direction() == Edge::Direction::BOTH)
        {
            addDirectedEdge(edgepair.second.id(), s, t, ix);
            ++ix;
        }

        if(edgepair.second.direction() == Edge::Direction::TO_FROM
        || edgepair.second.direction() == Edge::Direction::BOTH)
        {
            addDirectedEdge(edgepair.second.id(), t, s, ix);
            ++ix;
        }
    }
}

void
Graph::addDirectedEdge(EdgeIdType id,
                       const VertexType& source,
                       const VertexType& target,
                       EdgeIdType ix)
{
    const auto& res = boost::add_edge(source, target, mGraph);
    if(res.second == true)
    {
        mIdToEdgeMap.insert({id, res.first});
        mGraph[res.first].graphEdgeId = ix;
        mGraph[res.first].topoEdgeId = id;
//        mEdgeToIdMap.insert({res.first, id});
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
