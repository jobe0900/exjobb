graph
=====

The `graph` package consists of classes for representing graphs.

## GraphBuilder
The GraphBuilder is responsible for buildign graphs and linegraphs. It takes a `Topology` and a `Configuration`and uses them for building a `graph` and `linegraph` based on [Boost adjacency lists](http://www.boost.org/doc/libs/1_54_0/libs/graph/doc/adjacency_list.html). The `GraphBuilder` class holds several `maps` that connects the original Edges and Vertices to those used internally in the Boost graphs, so that it is possible to backtrack information about those elements. The internal Boost types keeps some properties ["bundled"](http://www.boost.org/doc/libs/1_54_0/libs/graph/doc/bundles.html), instead of as "interior" properties.

The ordinary `graph` is a directed graph that connects the _**vertices**_ and _**edges**_ from the topology. The `linegraph` transforms that graph to an edge-based graph that turns the graph's edges into _**nodes**_ in the linegraph, and those edges are connected with _**lines**_.

### Topology
`Topology` is a class holding `Edges` and `Vertices` for the topology fetched from the `MapProvider`. It simply states which `Vertices` are connected by which `Edges`, without any costs or restrictions or directions. When created it validates that the `source` and `target` Vertices of the Edges actually exists in the topology.

### Edge
The Edge holds some relevant data from the topology. It has an `id`, and a field for which the original `osm_id` was before building the database topology. It also holds id to `source` Vertex and id to `target` Vertex, some data about the geometry and the "road" such as number of lanes, a structure for costs and optionally for restrictions.

### EdgeCost
The cost for travel among an edge is the number of seconds it takes. The base for this calculation is of course dependent on the length of the edge, and the speed. The speed an be set as an explicit `maxspeed` restriction, or by looking up the configuration for a `surface` if such is stated, else the speed is found by a look up for the default speed for the "highway type" (road category).

The travel time cost can than be modified by barriers, speed bumps, traffic lights ... on the edge (or points that can be applied on the edge).

### EdgeRestriction
The `EdgeRestriction` keeps track of restrictions that can be imposed on 
an edge. Those restrictions are:

- **Vehicle properties**: weight, height, length, width, maxspeed.
- **General access**: [OSM wiki for access](http://wiki.openstreetmap.org/wiki/Key:access).
- **Vehicle type access**: as for General access, but specified for a category of vehicles, such as `motorcar` or `goods`.
- **Barrier**: if the edge is blocked with some kind of barrier.
- **Turning restrictions**: [OSM wiki for turn restrictions](http://wiki.openstreetmap.org/wiki/Relation:restriction).
- **Disused**: if the edge (road) is marked as no longer in use.
- **NoExit**: if the edge has no exit, it should not be used for building a linegraph.

(Turn restriction via other edges and not just via a vertex are difficult. At the time when converting the topology to a line graph it is impossible to have the relevant information. The solution is to set a flag on the Edge that there exist a VIA_WAY restriction that must be taken into account when routing, and the routing module must look up and make its own decisions somehow.)

### Vertex
The Vertex class is simple with just an `id` and a `point` location.

### TurnCostCalculator
This is a static helper class that assist in calculating the cost of making turns, when transforming the _graph_ into a _linegraph_. The cost for making turns are dependent on the angle of the turn, the category of the roads and the size of the vehicle. Other factors can be added. 

### Exceptions
`GraphException` is the main public exception to be thrown from this package. `RestrictionsException` and `TopologyException` are thrown when building those classes, but not as exposed externally.