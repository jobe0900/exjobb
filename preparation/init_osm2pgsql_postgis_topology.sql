CREATE EXTENSION postgis;
CREATE EXTENSION postgis_topology;
CREATE EXTENSION hstore;

DROP TABLE IF EXISTS turning_restrictions;
CREATE TABLE turning_restrictions(
   from_osm_id       bigint,   
   to_osm_id         bigint,
   via_osm           varchar,
   edge_ids          integer[],
   restriction_type  varchar);

--
-- Find the topology edge ids affected by osm turn restrictions.
--
CREATE OR REPLACE FUNCTION 
   find_topo_edges_at_turning_restriction(
      osm_edges_table text,
      from_osm_id bigint,
      to_osm_id bigint, 
      topo_edges_table text) 
RETURNS setof RECORD
AS $$
BEGIN
   RETURN QUERY EXECUTE format('  
      SELECT edge_id
      FROM %4$s
      WHERE ST_DWithin (
         geom,
         (   SELECT ST_Intersection(a.way, b.way)
             FROM %1$I a, %1$I b
             WHERE a.osm_id = %2$s AND b.osm_id = %3$s
         ),
         1.0
      );'
      , osm_edges_table, from_osm_id, to_osm_id, topo_edges_table);
END;
$$ LANGUAGE 'plpgsql';


--
-- Find all the restrictions and put them in table 'turning_restrictions'
--
CREATE OR REPLACE FUNCTION find_osm_turning_restrictions(osm_edges_table text, topo_edges_table text)
RETURNS integer 
AS $$
DECLARE
   nrFindings integer := 0;
   nrFrom integer := 0;
   nrTo integer := 0;
   restrictionRecord  record;
   ix integer;
   id bigint;
   fromOsmId bigint;
   toOsmId bigint;
   viaText text := '';
   restrictions text[] := '{
            "no_right_turn",
            "no_left_turn",
            "no_u_turn",
            "no_straight_on",
            "only_right_turn",
            "only_left_turn",
            "only_straight_on",
            "no_entry",
            "no_exit"
         }';
   restrType text;
   edgeId integer;
   edges integer[];
   
BEGIN
   FOR restrictionRecord IN
      SELECT * 
      FROM planet_osm_rels
      WHERE (
         SELECT 'restriction' = ANY(tags)
      )
      AND ( -- check that the restriction type is given in tags
         SELECT restrictions && tags
      )
   LOOP
      -- look through 'members' in all restrictions, must have at least 6 elements
      -- {from_id, from, via_id, via, to_id, to}
      IF (array_upper(restrictionRecord.members, 1) >= 6) THEN

         nrFrom := 0;
         nrTo := 0;
      
         -- look for type: from, via, to
         FOR ix IN 1..(array_length(restrictionRecord.members, 1)-1)
         LOOP
            IF restrictionRecord.members[ix+1] LIKE 'from' THEN 
               fromOsmId := 
                  trim(leading 'wn' from restrictionRecord.members[ix])::bigint;
               nrFrom := nrFrom + 1;
	        ELSIF restrictionRecord.members[ix+1] LIKE 'to' THEN 
	           toOsmId := 
	              trim(leading 'wn' from restrictionRecord.members[ix])::bigint;
	           nrTo := nrTo + 1;
	        ELSIF restrictionRecord.members[ix+1] LIKE 'via' THEN 
	           viaText := viaText || restrictionRecord.members[ix] || ',';
	        END IF;
         END LOOP;

         IF (nrFrom != 1 OR nrTo != 1) THEN
            CONTINUE;
         END IF;
         
         -- look for restriction type
         FOR ix IN 1..array_upper(restrictions, 1)
         LOOP
	        IF (SELECT restrictions[ix] = ANY(restrictionRecord.tags)) THEN
	           restrType := restrictions[ix];
	           EXIT;
	        END IF;
         END LOOP;

        -- find topology edge ids that might be affected
        -- each osm edge could have two topology edges (in and out at vertex)
        -- and there is no really easy way of finding who is who?
        FOR edgeId IN 
	       SELECT * 
	       FROM find_topo_edges_at_turning_restriction(
	          osm_edges_table, 
	          fromOsmId, 
	          toOsmId, 
	          topo_edges_table) 
	       AS f(id integer)
	    LOOP
	       edges := array_append(edges, edgeId);
	    END LOOP;

         -- store findings
         INSERT INTO turning_restrictions
            VALUES (fromOsmId, toOsmId, viaText, edges, restrType);
         nrFindings :=  nrFindings + 1;
      END IF;
   END LOOP;
   RETURN nrFindings;
END;
$$ LANGUAGE 'plpgsql';
