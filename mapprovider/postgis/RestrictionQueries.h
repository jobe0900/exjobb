/**  Queries for PostGisProvider to find restrictions.
 *
 * #include "RestrictionQueries.h"
 *
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_RESTRICTIONQUERIES_H_
#define MAPPROVIDER_POSTGIS_RESTRICTIONQUERIES_H_

// SYSTEM INCLUDES
//
#include <string>
#include <sstream>
#include <vector>

// PROJECT INCLUDES
//
#include <boost/algorithm/string.hpp>
#include <pqxx/pqxx>

// LOCAL INCLUDES
//
#include "../../osm/OsmHighway.h"
#include "../../osm/OsmTurningRestriction.h"
#include "../../graph/Edge.h"
#include "../../graph/Topology.h"
#include "../../graph/Vertex.h"
#include "../MapProviderException.h"

/** Class for holding static queries about restrictions,
 * needed by the PostGisProvider.
 */
class RestrictionQueries
{
public:
// TYPES
    struct VehiclePropertiesRestrictions
    {
        enum Columns
        {
            EDGE_ID,
            ELEMENT_ID,
            MAXHEIGHT,
            MAXLENGTH,
            MAXWEIGHT,
            MAXWIDTH,
            MAXSPEED,
            MINSPEED,
        };
    };

    struct AccessRestrictions
    {
        enum Columns
        {
            EDGE_ID,
            ELEMENT_ID,
            ACCESS,
            BARRIER,
            DISUSED,
            NOEXIT,
            MOTORCAR,
            GOODS,
            HGV,
            PSV,
            LHV,
            MOTOR_VEHICLE,
            VEHICLE,
        };
    };

    struct TurningRestrictions
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

    struct EdgePointRestrictions
    {
        enum Columns
        {
            POINT_OSM_ID,
            BARRIER,
            ACCESS,
            GOODS,
            HGV,
            LHV,
            MOTORCAR,
            MOTOR_VEHICLE,
            PSV,
            VEHICLE,
            EDGE_ID,
        };
    };

    /** Results from queries are handled by these functions. */
    struct Results
    {
        /** Parse a row in the results from turning restrictions.
         * @param   rRow        The row with results.
         * @param   rTopology   The topology that needs to be queried.
         * @return  A turning restriction object.
         * @throw   pqxx::pqxx_exception
         * @throw   TopologyException
         * @throw   MapProviderException
         */
        static OsmTurningRestriction*
                            parseTurningRestrictionResultRow(
            const pqxx::tuple&    rRow,
            Topology&             rTopology);


        /** Try to parse the column 'edge_ids' from the 'turning_restrictions'.
         * Split the string of edge ids and convert them to a vector of EdgeIds.
         * @param   rEdgeIds    A string like "{123, 456}". Gets trimmed of {}.
         * @return  A vector of the EdgeIds separately.
         * @throw   std::invalid_argument
         * @throw   std::out_of_range
         */
        static std::vector<EdgeIdType>
                            parseEdgeIdsString(std::string& rEdgeIds);

        /** Find the Edge that matches the OsmId in turning restriction.
         * @param   osmId       The original edge osm id.
         * @param   rEdgeIds    The candidate edges that are near restriction.
         * @param   rTopology   The Topology to ask for edges.
         */
        static Edge&        findEdgeMatchingOsmId(
            OsmIdType                       osmId,
            const std::vector<EdgeIdType>&  rEdgeIds,
            Topology&                       rTopology);
    };

// LIFECYCLE
    RestrictionQueries() = delete;
    RestrictionQueries(const RestrictionQueries& from) = delete;
    ~RestrictionQueries() = default;

// OPERATORS
// OPERATIONS
    /** Query for restrictions based on Vehicle Properties.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @param   rTopoEdgeTable  Name of table with topology edges.
     * @param   rOsmEdgeTable   Name of table with OSM edges.
     * @param   rSchemaName     Name of the topology schema.
     * @throw   pqxx::pqxx_exception
     */
    static void             getVehiclePropertyEdgeRestrictions(
        pqxx::transaction_base& rTrans,
        pqxx::result&           rResult,
        const std::string&      rTopoEdgeTable,
        const std::string&      rOsmEdgeTable,
        const std::string&      rSchemaName);

    /** Query for general access restrictions.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @param   rTopoEdgeTable  Name of table with topology edges.
     * @param   rOsmEdgeTable   Name of table with OSM edges.
     * @param   rSchemaName     Name of the topology schema.
     * @throw   pqxx::pqxx_exception
     */
    static void             getAccessRestrictions(
        pqxx::transaction_base& rTrans,
        pqxx::result&           rResult,
        const std::string&      rTopoEdgeTable,
        const std::string&      rOsmEdgeTable,
        const std::string&      rSchemaName);

    /** Drop and create the table 'turning_restrictions'.
     * @param   rTrans          Transaction to perform query in.
     * @throw   pqxx::pqxx_exception
     */
    static void             dropCreateTurningRestrictionsTable(
        pqxx::transaction_base& rTrans);

    /** Populate the table 'turning_restrictions'.
     * @param   rTrans          The transaction to execute within.
     * @param   rOsmEdgeTable   The name of the table with original osm edges.
     * @param   rTopoEdgeTable  The name of the table with topology edges.
     * @throw   pqxx::pqxx_exception
     */
    static void             identifyTurningRestrictions(
        pqxx::transaction_base& rTrans,
        const std::string&      rOsmEdgeTable,
        const std::string&      rTopoEdgeTable);

    /** Get the restrictions from the 'turning_restrictions' table.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @throw   pqxx::pqxx_exception
     */
    static void             getTurningRestrictions(
        pqxx::transaction_base& rTrans,
        pqxx::result&           rResult);

    /** Get the restrictions from the 'planet_osm_point' that relates to edges.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @param   rOsmPointTabl   The name of the table with original osm points.
     * @param   rTopoEdgeTable  The name of the table with topology edges.
     * @throw   pqxx::pqxx_exception
     */
    static void             getEdgePointRestrictions(
        pqxx::transaction_base& rTrans,
        pqxx::result&           rResult,
        const std::string&      rOsmPointTable,
        const std::string&      rTopoEdgeTable);
// ACCESS
// INQUIRY

protected:
private:
    /** SELECT FROM JOIN */
    static std::string  startOfQuery(const std::string& rTopoEdgeTable);
    /** Which columns to pick */
    static std::string  queryColumns(const std::vector<std::string>& rCols);
    /** FROM JOIN ON WHERE */
    static std::string  midOfQuery(
        const std::string& rSchemaName,
        const std::string& rOsmEdgeTable);
    /** Make sure only to pick rows with content in some column. */
    static std::string  notNullColumns(const std::vector<std::string>& rCols);
    /** AS ON ORDER BY */
    static std::string  endOfQuery();
};




#endif /* MAPPROVIDER_POSTGIS_RESTRICTIONQUERIES_H_ */
