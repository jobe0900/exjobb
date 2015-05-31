/**  Data structure for edges in Topology.
 *
 * #include "PostGisRestrictionQueries.h"
 *
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_POSTGISRESTRICTIONQUERIES_H_
#define MAPPROVIDER_POSTGIS_POSTGISRESTRICTIONQUERIES_H_

#include <string>
#include <sstream>
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

    /** Query for restrictions based on Vehicle Properties.
     * @param   rTrans          Transaction to perform query in.
     * @param   rTopoEdgeTable  Name of table with topology edges.
     * @param   rOsmEdgeTable   Name of table with OSM edges.
     * @param   rSchemaName     Name of the topology schema.
     * @result  pqxx::result of the query
     * @throw   pqxx::pqxx_exception
     */
    static pqxx::result     getVehiclePropertyEdgeRestrictions(
        pqxx::transaction_base& rTrans,
        const std::string&      rTopoEdgeTable,
        const std::string&      rOsmEdgeTable,
        const std::string&      rSchemaName)
    {
        std::vector<std::string> columns {
            "maxheight",
            "maxlength",
            "maxweight",
            "maxwidth",
            "maxspeed",
            "minspeed"
        };

        return rTrans.exec(
            startOfQuery(rTopoEdgeTable) +
            queryColumns(columns) +
            midOfQuery(rSchemaName, rOsmEdgeTable) +
            notNullColumns(columns) +
            endOfQuery()
        );
    }

    /** Query for general access restrictions.
     * @param   rTrans          Transaction to perform query in.
     * @param   rTopoEdgeTable  Name of table with topology edges.
     * @param   rOsmEdgeTable   Name of table with OSM edges.
     * @param   rSchemaName     Name of the topology schema.
     * @result  pqxx::result of the query
     * @throw   pqxx::pqxx_exception
     */
    static pqxx::result     getAccessRestrictions(
        pqxx::transaction_base& rTrans,
        const std::string&      rTopoEdgeTable,
        const std::string&      rOsmEdgeTable,
        const std::string&      rSchemaName)
    {
        std::vector<std::string> columns {
            "access",
            "barrier",
            "disused",
            "noexit",
            "motorcar",
            "goods",
            "hgv",
            "psv",
            "lhv",
            "motor_vehicle",
            "vehicle"
        };

        return rTrans.exec(
            startOfQuery(rTopoEdgeTable) +
            queryColumns(columns) +
            midOfQuery(rSchemaName, rOsmEdgeTable) +
            notNullColumns(columns) +
            endOfQuery()
        );
    }

    /** Drop and create the table 'turning_restrictions'.
     * @param   rTrans          Transaction to perform query in.
     * @throw   pqxx::pqxx_exception
     */
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
     * @throw   pqxx::pqxx_exception
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

    /** Get the restrictions from the 'turning_restrictions' table.
     * @param   rTrans          Transaction to perform query in.
     * @return  pqxx::result    The turning restrictions.
     * @throw   pqxx::pqxx_exception
     */
    static pqxx::result     getTurningRestrictions(
        pqxx::transaction_base& rTrans)
    {
         return rTrans.exec(
            "SELECT * FROM turning_restrictions"
        );
    }

private:
    static std::string startOfQuery(const std::string& rTopoEdgeTable)
    {
        return (
            "SELECT     edge_id, "
            //-- osm data about original edge
            "           osm.* "
            "FROM      " + rTopoEdgeTable +
            " JOIN ( "
            "   SELECT  element_id "
        );
    }

    static std::string midOfQuery(
        const std::string& rSchemaName,
        const std::string& rOsmEdgeTable)
    {
        return (
            "   FROM    " + rSchemaName + ".relation "
            "   JOIN    " + rOsmEdgeTable +
            "   ON      topogeo_id = (topo_geom).id "
            "   WHERE   highway in " + OsmHighway::typesAsCommaSeparatedString()
        );
    }

    static std::string endOfQuery()
    {
        return (
            ") AS osm "
            "ON edge_id = element_id "
            "ORDER BY edge_id ASC;"
        );
    }

    static std::string queryColumns(const std::vector<std::string>& rCols)
    {
        std::ostringstream oss;
        for(const std::string& col : rCols)
        {
            oss << ", " << col;
        }
        return oss.str();
    }

    static std::string notNullColumns(const std::vector<std::string>& rCols)
    {
        std::ostringstream oss;
        oss << " AND (";
        size_t i = 0;
        for(const std::string& col : rCols)
        {
            oss << col << " IS NOT NULL ";
            if(i < (rCols.size() - 1))
            {
                oss << " OR ";
            }
            ++i;
        }
        oss << ") ";
        return oss.str();
    }
};




#endif /* MAPPROVIDER_POSTGIS_POSTGISRESTRICTIONQUERIES_H_ */
