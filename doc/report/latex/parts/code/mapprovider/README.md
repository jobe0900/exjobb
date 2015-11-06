MapProvider
===========

The `mapprovider` package exists to implement different classes that can provide access to OpenStreetMap data.

#### Background
There exists several solutions to import OpenStreetMap data into a database, and the different solutions all creates different schemas and tables. To keep the flexibility to change how we get the OSM data, the `mapprovider` exists to provide an abstract interface that providers must implement.

#### `jsontest`
The `JsonTestProvider` is a small map provider that was implemented to be able to read in a small set of well-known edges and vertices (such as the [pgRouting sample data](http://docs.pgrouting.org/dev/doc/src/developer/sampledata.html)), to be used under development of the `Graph` class.

#### `postgis`
The `PostGisProvider` exists for working with topologies built with the `postgis_topology` extension. This is the course taken during development of the `LineGraphUtility` so far. (Using `pgRouting` seemed to be useful only for building topology and not to get access to the other map data, and `osm2po` is not open source. But with the `MapProvider` interface it is possible to implement another if desirable.)

The `PostGisProvider` uses classes `TopologyQueries` and `RestrictionQueries` for querying the database.

### Exceptions
Each subpackage throws its own exception: `MapProviderException`, and `PostGisProviderException`.
