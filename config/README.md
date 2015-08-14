
Configuration
=============

Configurations are set in the file `settings.json`. The different parts of the configuration is:

- Database
- Topology
- Vehicle
- Access
- Restrictions
- Costs

If one wishes to edit the setting, one can freely add and remove objects in _braces_ (`[` and `]`), for example if one wishes to edit which values for a tag inflicts a cost. But the strings in _curly braces_ (`{` and `}`) are keys that must exist, but the values for the keys can of course be edited.


Database
--------

Configuration for connecting to the database holding map data. The expected keys and values are:

- **"host"**:
    - *hostname* (e.g. `"localhost"`) or  
    - *ip-address* (e.g. `"127.0.0.1"`).
- **"port"**:
    - *portnumber* (e.g. `5432`).
- **"username"**:
    - *username* (e.g. `"tester"`).
- **"password"**:
    - *password* (e.g. `"tester_pass"`).
- **"database"**:
    - *database name* (e.g. `"db_name"`).
    
    
Topology
--------

Configurations for building or reading topology from a database. Might have different meanings depeding on which *MapProvider* are used. Topologies can be prebuilt, or they can be generated each time, depending on the settings in `"build_topo"`. It is also possible to define a simple json test file, for testing simple topologies.

- **"provider"**:
    - *name* of *MapProvider*
        - `"postgis"` when using `postgis_topology` for building topologies.
        - `"pgrouting"` when using `pgrouting` for building toplogies. 
        - `"jsontest"` for simple json test topology.

- **"postgis"**:  
    - **"topo_name"**:
        - *basename* for prebuilt topologies (e.g. `"test"`), combined with `roads_prefix` and `topo_prefix` for actual names such as `"highways_test"` and `"topo_test"`.
    
    - **"roads_prefix"**:
        - *prefix* to add to `topo_name` (e.g. `"highways"`) for table of roads network, see above.

    - **"schema_prefix"**:
        - *prefix* to add to `topo_name` (e.g. `"topo"`) for schema with topology data when using `postgis_topology`, see above.
        
    - **"build"**:
        - **"temp_topo_name**:
            - `""` (*empty*) if not building temporary topologies.
            - `"epoch_ms"` for adding a string with the count of milliseconds since "Epoch" as the `topo_name`.
        
        - **"srid"**:
            - *number* identifying which projection to use.
                - `900913` for geometric metrical projection, unit meters.
                - `4326` for geographic spherical projection, unit degrees.
    
        - **"tolerance"**:
            - *snapping* of nodes in unit of projection when building topology, e.g. 1.0 for srid 900913, or 0.001 for srid 4326.
        
    - **"edge"**:
    
        - **"table"**:
            - *name* of the table with topology edges (e.g. `"edge_data"`), with column for id, source, target and geometry.
    
        - **"id_col"**:
            - *name* of the column in edge table with id of edges (e.g. `"edge_id"`).
    
        - **"source_col"**:
            - *name* of the column in edge table with vertex id of **source** of edge (e.g. `"start_node"`).
    
        - **"target_col"**:
            - *name* of the column in edge table with vertex id of **target** of edge (e.g. `"end_node"`).
    
        - **"geom_col"**:
            - *name* of the column in edge table with geometry of edge (e.g. `"geom"`).
        
    - **"vertex"**:
        - **"table"**:
            - *name* of the table with topology vertices (e.g. `"node"`), with column for id and geometry.
            
        - **"id_col"**:
            - *name* of the column in vertex table with id of vertices (e.g. `"node_id"`).
            
        - **"geom_col"**:
            - *name* of the column in vertex table with geometry of vertex (e.g. `"geom"`).
            
- **"pgrouting"**:
    - TODO.
    
- **"jsontest"**:
    - **"test_file"**:
        - `""` (*empty*) if not using *json-test provider*.
        - *filename* to a json test-file (e.g. `"test.json"`) looking like:
    
    ```json
    {
        "vertices": [
            [1,2,0],
            [2,2,1]
        ],
        "edges": [
            [1,1,2,0]
        ]
    }
    ```
    
        where each row in `vertices` are `[id,x,y]` and each row in `edges` are `[id, source vertex id, target vertex id, direction]`. Values for `direction` is `0 = BOTH`, `1 = FROM_TO`, `2 = TO_FROM`. 
        

Vehicle
-------

Configuration about the vehicle to route through the topology. Information might be needed to take restrictions in account.

- **"category"**:
    - *name* of OSM catagory of the vehicle. [OSM Access](http://wiki.openstreetmap.org/wiki/Key:access). (E.g. "motorcar"). Definition of the category must state dimensions as below.
- **_"category_name"_**:
    - *height* of vehicle in meters.
    - *length* of vehicle in meters.
    - *width* of vehicle in meters.
    - *weight* of vehicle in tons.
    - *maxspeed* of vehicle in km/h.
    - *acceleration* is the time it takes from 0 to 100 km/h.
    - *deceleration* is the time it takes from 100 to 0 km/h.


Access
------

- **"allow"**:
    List of which values for tag `access` that permits access. Those values for `access` that are not listed here are considered to restrict access.
    
    
Restrictions
------------

- **"barriers"**:
    - List of which values for `barriers` that restricts access. Those values not listed are assumed to allow access.


Cost
----

Configuration relating to costs when routing through the graph.

- **"default_speed"**:
    - each road category has default speeds when none is specified. [OSM default speeds](http://wiki.openstreetmap.org/wiki/OSM_tags_for_routing/Maxspeed). Most roads have two speeds, `high` and `low`, which differentiate the speeds inside and outside of a town. `living_street` is always inside so only the low is important. `motorway` is really the `high` number, and the `low` number is the speed on the links (ramps). It is not trivial to find out if a road is inside or outside of that area, so for this application which is meant to be used for routing in urban areas (?), the `low` number is assumed for all cost calculations.
- **"surface"**:
    - each surface type is associated with a max speed in km/h over which one should not drive.
- **"barriers"**:
    - this is a list of which barriers causes a slow-down, and the number of seconds it is  probable it takes to pass.
- **"highway"**:
    - a list of values for the `highway` tag that can mean a time cost in seconds, such as zebra crossings, bus stops, stop or give way sign, and more.
- **"railway"**:
    - a list of values for the `railway` tag that can mean a time cost in seconds, such as a crossing between railway and highway (`level_crossing`).
- **"public_transport"**:
    - a list of values for the `public_transport` tag that can mean a time cost in seconds, such as a `stop_position` if one thinks it is suitable to slow down when passing a bus stop.
- **"traffic_calming"**:
    - a list of traffic calming objects and their time costs in seconds.
    