PostGisProvider
===============

This module fetches map data from a PostGis database where the OSM data has been imported with `osm2pgsql`, and topology has been built with the `postgis_topology` extension.

It fetches data from the database, converts it to valid Vertex and Edge types and stores them in a Toplogy.

Edges
-----

The Edges are fetched with a lot of columns. Some of the data are stored as attributes in the Edge class, some in the associated data structures (such as GeomData, RoadData) and others are used for checking prior to adding edge to topology (restrictions based on vehicle propoerties such as width and weight).