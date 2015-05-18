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
        const auto& v = *(v_it.first);
        VertexIdType v_id = rGraph.mVertexToIdMap.at(v);
        const Vertex& vertex = rGraph.mrTopology.getVertex(v_id);
        os << "\t\t v: " << v << "\t: " << vertex << std::endl;
    }

    for(auto e_it = boost::edges(rGraph.mGraph);
        e_it.first != e_it.second;
        ++e_it.first)
    {
        const auto& e = *(e_it.first);
        EdgeIdType e_id = rGraph.mEdgeToIdMap.at(e);
        const Edge& edge = rGraph.mrTopology.getEdge(e_id);
        os << "\t\t e: " << e << "\t: " << edge << std::endl;
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
//    for(auto it = mrTopology.vertexMap.begin();
//        it != mrTopology.vertexMap.end();
//        ++it)
//    {
//        VertexType v = boost::add_vertex(mGraph);
//        mIdToVertexMap.insert({it->second.id(), v});
//        mVertexToIdMap.insert({v, it->second.id()});
//    }

    for(const auto& vertexpair : mrTopology.vertexMap)
    {
        VertexType v = boost::add_vertex(mGraph);
        mIdToVertexMap.insert({vertexpair.second.id(), v});
        mVertexToIdMap.insert({v, vertexpair.second.id()});
    }
}

void
Graph::addTopoEdgesToGraph()
{
    for(const auto& edgepair : mrTopology.edgeMap)
    {
        const VertexType& s = getGraphVertex(edgepair.second.source());
        const VertexType& t = getGraphVertex(edgepair.second.target());

        if(edgepair.second.direction() == Edge::Direction::FROM_TO
        || edgepair.second.direction() == Edge::Direction::BOTH)
        {
            addDirectedEdge(edgepair.second.id(), s, t);
        }

        if(edgepair.second.direction() == Edge::Direction::TO_FROM
        || edgepair.second.direction() == Edge::Direction::BOTH)
        {
            addDirectedEdge(edgepair.second.id(), t, s);
        }
    }
}

void
Graph::addDirectedEdge(EdgeIdType id,
                       const VertexType& source,
                       const VertexType& target)
{
    const auto& res = boost::add_edge(source, target, mGraph);
    if(res.second == true)
    {
        mIdToEdgeMap.insert({id, res.first});
        mEdgeToIdMap.insert({res.first, id});
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
