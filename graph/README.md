README
======

graph
-----

The `graph` package constist of classes for representing graphs.

### Vertex
The Vertex class is simple with just an `id` and a `point` location.

### Edge
The Edge class simply has an `id`, and id to `source` Vertex and id to `target` Vertex.

### Topology
Topology is a class holding Edges and Vertices for the topology fetched from the `MapProvider`. It simply states which Vertices are connected by which Edges, without any costs or restrictions or directions. When created it validates that the `source` and `target` Vertices of the Edges actually exists in the topology.

### Graph
A Graph consists of a `Topology` and `Costs` (TODO) and `Restrictions` (TODO). The Graph is directed, so when one adds a topology it creates a directed Boost graph as the representation. The Graph maps "Graph Edges" and "Graph Vertices" to id's to the Edge class and the Vertex class, so one can have a look up to find all relevant information in the original Edges and Vertices in the Topology.


To enable both ordinary graphs and line graphs to use the same graph class, both types uses the same classes for graph, topology and vertices and edges. All differing data is found in `EdgeData` and `VertexData`. Those classes are abstract, and the actual specialization is found in the sub-classes `TopoEdgeData`, `LineGraphEdgeData` and `TopoVertexData` and `LineGraphVertexData`.