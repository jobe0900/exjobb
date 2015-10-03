PostGisProvider
===============

This is a concrete class that implements the MapProviders interface. It fetches map data from a PostGis database where the OSM data has been imported with `osm2pgsql`, and topology has been built with the `postgis_topology` extension, converts it to valid `Vertex` and `Edge` types and stores them in a `Topology`.

The handling of the queries are factored out in different static classes:
- `CostQueries` for handling costs.
- `RestrictionQueries` for handling restrictions.
- `TopologyQueries` for handling topology related stuff.
- `LineGraphSaveQueries` for persisting a LineGraph back to the database.