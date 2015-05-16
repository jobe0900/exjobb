/*
 * Topology.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Topology.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Topology::Topology(const TopoVertexMapType& rVertexMap,
                   const TopoEdgeMapType& rEdgeMap)
    : mrVertexMap(rVertexMap),
      mrEdgeMap(rEdgeMap)
{ }
//Topology::Topology(const MapProvider& rMapProvider)
//    : mrMapProvider(rMapProvider)
//{ }

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//const Vertex&
//Topology::addVertex(TopologyId id, Point point)
//{
//    auto res = mVertexMap.emplace(id, Vertex(id, point));
//    return res.first->second;
//}


//const TopologyEdge&
//Topology::addEdge(EdgeId id, VertexIdType source, VertexIdType target)
//{
// 	try
//	{
//	    getVertex(source);
//	    getVertex(target);
//		auto res = mEdgeMap.emplace(id, TopologyEdge(id, source, target));
//		return res.first->second;
//	}
//	catch (TopologyException& e)
//	{
//		throw TopologyException("Cannot add edge: " + std::to_string(id) +
//				". " + e.what());
//	}
//}


const Vertex&
Topology::getVertex(VertexIdType id) const
{
    auto it = mrVertexMap.find(id);
    if(it == mrVertexMap.end()) {
        throw TopologyException("Vertex not found: " + std::to_string(id));
    }
    return it->second;
}


//============================= ACESS      ===================================
size_t
Topology::nrVertices() const
{
    return mrVertexMap.size();
}

size_t
Topology::nrEdges() const
{
    return mrEdgeMap.size();
}
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////




