CREATE TABLE highways_lgu 
AS SELECT * 
FROM planet_osm_line 
WHERE highway IS NOT NULL;

SELECT topology.CreateTopology('topo_lgu', 900913);

SELECT topology.AddTopoGeometryColumn('topo_lgu', 
                                      'public', 
                                      'highways_lgu', 
                                      'topo_geom', 
                                      'LINESTRING');

UPDATE highways_lgu SET topo_geom = topology.toTopoGeom(way, 'topo_lgu', 1, 1.0);

