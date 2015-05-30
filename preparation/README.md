Preparing database
==================

The preparation differs depending on how you import the map data and build the topology. So far in this project import has been done with `osm2pgsql` and topology built with `postgis_topology`.


Preparation for `osm2pgsql` and `postgis_topology`
----------------------------------------------------- 

To prepare the database for this software module when using `osm2pgsql` as importer of OpenStreetMap data into the database, we need to install extensions for:

- `postgis`
- `postgis_topology`
- `hstore`

and a couple of custom functions for finding turning restrictions:

- function `find_topo_edges_at_turning_restriction()`
- function `find_osm_turning_restrictions()`

The steps to follow are (assuming `mikhailovsk.osm` as source for OpenStreetMap data, and `tester` as a user with administrative rights in database, and `mikh_0530` as name of database):

### 1. Create database

    $ createdb mikh_0530 -U tester

### 2. Install extensions and functions

    $ psql -U tester -d mikh_0530 -f init_osm2pgsql_postgis_topology.sql
    
### 3. Import OSM data

    $ osm2pgsql -U tester -d mikh_0530 -s -k -S LGU.style mikhailovsk.osm
    
This uses the tool `osm2pgsql` to parse the osm-file into database tables.
The flags are

- `-s` slim, keeping extra tables.
- `-k` keeping tags in `hstore` if not in their own column.
- `-S` Style file, configuring which tags to have columns or not.
    
### 4. Building topology

This step is optional. It should be efficient and safe to build the topology once and for all after importing as differing conditions and temporary closures could be specified with costs and restrictions instead of via topology. But one can also configure the tool to build topology on each call, see the `configuration` package.

    $ psql -U tester -d mikh_0530 -f build_postgis_topology.sql
    
This step creates a table `public.highways_lgu` and adds a new schema called `topo_lgu` which contains tables for the topology.
