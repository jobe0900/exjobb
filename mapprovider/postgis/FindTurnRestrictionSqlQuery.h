/**  Provide queries to PostGis database to get map data.
 *
 * #include "FindTurnRestrictionSqlQuery.h"
 *
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_FINDTURNRESTRICTIONSQLQUERY_H_
#define MAPPROVIDER_POSTGIS_FINDTURNRESTRICTIONSQLQUERY_H_

#include <pqxx/pqxx>
#include <string>

struct TurningRestrictionsColumns
{
    enum Columns
    {
        FROM_OSM_ID,
        TO_OSM_ID,
        VIA_OSM,
        EDGE_IDS,
        RESTRICTION_TYPE
    };
};

struct FindTurnRestrictionSqlQuery
{
    static void     findOsmTurningRestrictions(
        pqxx::transaction_base& rTransaction,
        const std::string&      rOsmEdgeTable,
        const std::string&      rTopoEdgeTable)
    {
        std::string sql("\
DROP TABLE IF EXISTS turning_restrictions;\
CREATE TABLE turning_restrictions(\
   from_osm_id       bigint,\
   to_osm_id         bigint,\
   via_osm           varchar,\
   edge_ids          integer[],\
   restriction_type  varchar);\
\
--\
-- Find all the restrictions and put them in table 'turning_restrictions'\
--\
CREATE OR REPLACE FUNCTION find_osm_turning_restrictions()\
RETURNS integer AS\
$$\
DECLARE\
   nrFindings integer := 0;\
   nrFrom integer := 0;\
   nrTo integer := 0;\
   restrictionRecord  record;\
   ix integer;\
   id bigint;\
   fromOsmId bigint;\
   toOsmId bigint;\
   viaText text := '';\
   restrictions text[] := '{\
            \"no_right_turn\",\
            \"no_left_turn\",\
            \"no_u_turn\",\
            \"no_straight_on\",\
            \"only_right_turn\",\
            \"only_left_turn\",\
            \"only_straight_on\",\
            \"no_entry\",\
            \"no_exit\"\
         }';\
   restrType text;\
   edgeIds record;\
   edges integer[];\
\
BEGIN\
   FOR restrictionRecord IN\
      SELECT * \
      FROM planet_osm_rels\
      WHERE (\
         SELECT 'restriction' = ANY(tags)\
      )\
      AND ( -- check that the restriction type is given in tags\
         SELECT restrictions && tags\
      )\
   LOOP  -- through all restrictions\
      -- look through 'members', must have at least 6 elements\
      -- {from_id, from, via_id, via, to_id, to}\
      IF(array_upper(restrictionRecord.members, 1) >= 6) THEN\
\
         nrFrom := 0;\
         nrTo := 0;\
\
         -- look for type: from, via, to\
         FOR ix IN 1..(array_length(restrictionRecord.members, 1)-1)\
         LOOP\
            IF restrictionRecord.members[ix+1] LIKE 'from' THEN\
               fromOsmId := trim(leading 'wn' from restrictionRecord.members[ix])::bigint;\
               nrFrom := nrFrom + 1;\
        ELSIF restrictionRecord.members[ix+1] LIKE 'to' THEN \
           toOsmId := trim(leading 'wn' from restrictionRecord.members[ix])::bigint;\
           nrTo := nrTo + 1;\
        ELSIF restrictionRecord.members[ix+1] LIKE 'via' THEN \
              viaText := viaText || restrictionRecord.members[ix] || ',';\
        END IF;\
         END LOOP;\
\
         IF (nrFrom != 1 OR nrTo != 1) THEN\
            CONTINUE;\
         END IF;\
\
         -- look for restriction type\
         FOR ix IN 1..array_upper(restrictions, 1)\
         LOOP\
        IF (SELECT restrictions[ix] = ANY(restrictionRecord.tags)) THEN\
           restrType := restrictions[ix];\
           EXIT;\
        END IF;\
         END LOOP;\
\
         -- find topology edge ids that might be affected\
         -- each osm edge could have two topology edges (in and out at vertex)\
         -- and there is no really easy way of finding who is who?\
         FOR edgeIds IN\
            (  SELECT edge_id\
               FROM " + rTopoEdgeTable + "  \
               WHERE ST_DWithin (\
                  geom,\
                  (   SELECT ST_Intersection(a.way, b.way)\
                      FROM " + rOsmEdgeTable + " a, " + rOsmEdgeTable + " b\
                      WHERE a.osm_id = fromOsmId AND b.osm_id = toOsmId\
                  ),\
                  1.0\
               )\
            )\
         LOOP\
            edges := array_append(edges, edgeIds.edge_id);\
         END LOOP;\
\
\
         -- store findings\
         INSERT INTO turning_restrictions\
            VALUES (fromOsmId, toOsmId, viaText, edges, restrType);\
         nrFindings :=  nrFindings + 1;\
      END IF;\
   END LOOP;\
   RETURN nrFindings;\
END;\
$$ LANGUAGE 'plpgsql';\
\
\
SELECT * FROM find_osm_turning_restrictions();\
\
                          ");
    rTransaction.exec(sql);
    }
};



#endif /* MAPPROVIDER_POSTGIS_FINDTURNRESTRICTIONSQLQUERY_H_ */
