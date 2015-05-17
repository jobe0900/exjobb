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
      mVertexMap(),
      mEdgeMap(),
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
    return mVertexMap.size();
}

size_t
Graph::nrEdges() const
{
    return mEdgeMap.size();
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
    const auto& it = mVertexMap.find(vertexId);
    return (it != mVertexMap.end());
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
Graph::addTopoVerticesToGraph()
{
    for(const auto& vertexpair : mrTopology.vertexMap)
    {
        VertexType v = boost::add_vertex(mGraph);
        mVertexMap.insert({vertexpair.second.id(), v});
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
                mEdgeMap.insert({edgepair.second.id(), res.first});
            }
        }

        if(edgepair.second.direction() == Edge::Direction::TO_FROM
        || edgepair.second.direction() == Edge::Direction::BOTH)
        {
            const auto& res = boost::add_edge(s, t, mGraph);
            if(res.second == true)
            {
                mEdgeMap.insert({edgepair.second.id(), res.first});
            }
        }
    }
}

const Graph::VertexType&
Graph::getGraphVertex(VertexIdType id) const
{
    const auto& res = mVertexMap.find(id);
    if(res == mVertexMap.end())
    {
        throw GraphException("Graph:getGraphVertex: Missing vertex: "
            + std::to_string(id));
    }
    return res->second;
}
