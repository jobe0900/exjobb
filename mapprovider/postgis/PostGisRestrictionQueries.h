/*
 * PostGisRestrictionQueries.h
 *
 *  Created on: 30 maj 2015
 *      Author: jonas
 */

#ifndef MAPPROVIDER_POSTGIS_POSTGISRESTRICTIONQUERIES_H_
#define MAPPROVIDER_POSTGIS_POSTGISRESTRICTIONQUERIES_H_

#include <string>
#include <vector>
#include <pqxx/pqxx>

#include "../../osm/OsmHighway.h"

/** Class for holding static queries needed by the PostGisProvider. */
struct PostGisRestrictionQueries
{
    enum TurningRestrictionsColumns
    {
        FROM_OSM_ID,
        TO_OSM_ID,
        VIA_OSM,
        EDGE_IDS,
        RESTRICTION_TYPE
    };

    static pqxx::result     getVehiclePropertyEdgeRestrictions(
        pqxx::transaction_base& rTrans,
        const std::string& rTopoEdgeTable,
        const std::string& rOsmEdgeTable,
        const std::string& rSchemaName)
    {
        return rTrans.exec(
            "SELECT     edge_id, "
            //-- osm data about original edge
            "           osm.* "
            "FROM      " + rTopoEdgeTable +
            " JOIN ( "
            "   SELECT  element_id "
            //-- vehicle property restrictions
            "           , maxheight "
            "           , maxlength "
            "           , maxweight "
            "           , maxwidth "
            "           , maxspeed "
            "           , minspeed "
            "   FROM    " + rSchemaName + ".relation "
            "   JOIN    " + rOsmEdgeTable +
            "   ON      topogeo_id = (topo_geom).id "
            "   WHERE   highway in " + OsmHighway::typesAsCommaSeparatedString() +
            "   AND     (maxheight IS NOT NULL "
            "   OR       maxlength IS NOT NULL "
            "   OR       maxweight IS NOT NULL "
            "   OR       maxwidth  IS NOT NULL "
            "   OR       maxspeed  IS NOT NULL "
            "   OR       minspeed  IS NOT NULL)"
            ") AS osm "
            "ON edge_id = element_id "
            "ORDER BY edge_id ASC;"
        );
    }

    /** Drop and create the table 'turning_restrictions'. */
    static void             dropCreateTurningRestrictionsTable(
        pqxx::transaction_base& rTrans)
    {
        rTrans.exec(
            "DROP TABLE IF EXISTS turning_restrictions; "
            "CREATE TABLE turning_restrictions( "
            "   from_osm_id       bigint, "
            "   to_osm_id         bigint, "
            "   via_osm           varchar, "
            "   edge_ids          integer[], "
            "   restriction_type  varchar)"
        );
    }

    /** Populate the table 'turning_restrictions'.
     * @param   rTrans          The transaction to execute within.
     * @param   rOsmEdgeTable   The name of the table with original osm edges.
     * @param   rTopoEdgeTable  The name of the table with topology edges.
     */
    static void             identifyTurningRestrictions(
        pqxx::transaction_base& rTrans,
        const std::string&      rOsmEdgeTable,
        const std::string&      rTopoEdgeTable)
    {
        rTrans.exec(
            "SELECT * FROM find_osm_turning_restrictions('"
            + rOsmEdgeTable + "', '" + rTopoEdgeTable + "')"
        );
    }

    /** Get the restrictions from the 'turning_restrictions' table. */
    static pqxx::result     getTurningRestrictions(
        pqxx::transaction_base& rTrans)
    {
         return rTrans.exec(
            "SELECT * FROM turning_restrictions"
        );
    }
};




#endif /* MAPPROVIDER_POSTGIS_POSTGISRESTRICTIONQUERIES_H_ */
