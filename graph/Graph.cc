/*
 * Graph.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

#include "Graph.h"  // class implemented
#include "TopoEdgeData.h"

#include <typeinfo>


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
    VertexIdType v_ix = 0;
    for(const auto& vertexpair : mrTopology.mVertexMap)
    {
        VertexType v = boost::add_vertex(mGraph);
        mIdToVertexMap.insert({vertexpair.second.id(), v});
        mGraph[v].graphVertexId = v_ix;
        mGraph[v].topoVertexId = vertexpair.second.id();
        ++v_ix;
//        mVertexToIdMap.insert({v, vertexpair.second.id()});
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


        if((e.edgeData() != nullptr)
        && (typeid(TopoEdgeData) == typeid(*e.edgeData())))
        {
            TopoEdgeData* p_ed = static_cast<TopoEdgeData*>(e.edgeData());

            if(p_ed->direction() == TopoEdgeData::Direction::FROM_TO
            || p_ed->direction() == TopoEdgeData::Direction::BOTH)
            {
                addDirectedEdge(e.id(), s, t, e_ix);
                ++e_ix;
            }

            if(p_ed->direction() == TopoEdgeData::Direction::TO_FROM
            || p_ed->direction() == TopoEdgeData::Direction::BOTH)
            {
                addDirectedEdge(e.id(), t, s, e_ix);
                ++e_ix;
            }
        }
        else
        {
            addDirectedEdge(e.id(), s, t, e_ix);
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
