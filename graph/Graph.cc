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
    : mGraph(rTopology.nrVertices()),
      mIdToVertexMap(),
      mIdToEdgeMap(),
      mrTopology(rTopology)
{
    addTopoVerticesToGraph();
    addTopoEdgesToGraph();
}

//============================= OPERATORS ====================================
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
    for(const auto& vertexpair : mrTopology.vertexMap)
    {
        VertexType v = boost::add_vertex(mGraph);
        mIdToVertexMap.insert({vertexpair.second.id(), v});
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
            const auto& res = boost::add_edge(s, t, mGraph);
            if(res.second == true)
            {
                mIdToEdgeMap.insert({edgepair.second.id(), res.first});
            }
        }

        if(edgepair.second.direction() == Edge::Direction::TO_FROM
        || edgepair.second.direction() == Edge::Direction::BOTH)
        {
            const auto& res = boost::add_edge(s, t, mGraph);
            if(res.second == true)
            {
                mIdToEdgeMap.insert({edgepair.second.id(), res.first});
            }
        }
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
