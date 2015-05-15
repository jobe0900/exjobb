/*
 * TopologyTypes.h
 *
 *  Created on: 13 maj 2015
 *      Author: jonas
 */

#ifndef GRAPH_TOPOLOGYTYPES_H_
#define GRAPH_TOPOLOGYTYPES_H_

#include <map>
#include <boost/graph/adjacency_list.hpp>

typedef int TopologyId;
typedef int VertexId;
typedef int EdgeId;

typedef std::map<TopologyId, Vertex>           TopoVertexMapType;

typedef std::map<TopologyId, Edge>                     TopoEdgeMapType;

typedef boost::adjacency_list
    <boost::listS, boost::vecS, boost::undirectedS>    UndirectedGraphType;

typedef boost::adjacency_list
    <boost::listS, boost::vecS, boost::directedS>      DirectedGraphType;

typedef boost::adjacency_list
    <boost::listS, boost::vecS, boost::bidirectionalS> BidirectedGraphType;

#endif /* GRAPH_TOPOLOGYTYPES_H_ */
