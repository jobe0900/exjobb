PostGisProvider
===============

This module fetches map data from a PostGis database where the OSM data has been imported with `osm2pgsql`, and topology has been built with the `postgis_topology` extension.

It fetches data from the database, converts it to valid Vertex and Edge types and stores them in a Toplogy.

Edges
-----

The Edges are fetched with a lot of columns. Some of the data are stored as attributes in the Edge class, some in the associated data structures (such as GeomData, RoadData) and others are used for checking prior to adding edge to topology (restrictions based on vehicle propoerties such as width and weight).


Access
------
It is pretty impossible to build an isolated line graph based on access restrictions, separate from the routing, as some access restrictions are base on whether the edge (road) is  your destination or a via route. That is something that is impossible to know separately from the routing.