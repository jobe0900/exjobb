PostGisProvider
===============

This module fetches map data from a PostGis database where the OSM data has been imported with `osm2pgsql`, and topology has been built with the `postgis_topology` extension.

It fetches data from the database, converts it to valid Vertex and Edge types and stores them in a Toplogy.
