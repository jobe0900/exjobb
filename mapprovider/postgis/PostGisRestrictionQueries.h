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
#include "../../graph/Edge.h"
#include "../../graph/Topology.h"
#include "../../graph/Vertex.h"

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
        static OsmTurningRestriction
                            parseTurningRestrictionResultRow(
            const pqxx::tuple&    rRow,
            Topology&             rTopology)
        {
            OsmIdType fromOsmId =
                rRow[PostGisRestrictionQueries::FROM_OSM_ID].as<OsmIdType>();
            OsmIdType toOsmId =
                rRow[PostGisRestrictionQueries::TO_OSM_ID].as<OsmIdType>();
            std::string typeString =
                rRow[PostGisRestrictionQueries::RESTRICTION_TYPE].as<std::string>();
            OsmTurningRestriction::TurningRestrictionType type =
                OsmTurningRestriction::parseString(typeString);
            std::string edgeIdsString =
                rRow[PostGisRestrictionQueries::EDGE_IDS].as<std::string>();

            std::vector<EdgeIdType> edgeIds = parseEdgeIdsString(edgeIdsString);

            Edge& fromEdge = findEdgeMatchingOsmId(fromOsmId, edgeIds, rTopology);
            Edge& toEdge   = findEdgeMatchingOsmId(toOsmId, edgeIds, rTopology);

            OsmTurningRestriction::TurningViaType viaType =
                OsmTurningRestriction::VIA_NODE;

            if(fromEdge.target() != toEdge.source())
            {
                viaType = OsmTurningRestriction::VIA_WAY;
            }

//            VertexIdType vertex = findCommonVertexId(edgeIds, rTopology);

            // if turning restriction is via way, then the affected edge ids should only contain
            // the from edge, as there is no intersection with the to edge.+
        }
        /** Try to parse the column 'edge_ids' from the 'turning_restrictions'.
         * Split the string of edge ids and convert them to a vector of EdgeIds.
         * @param   rEdgeIds    A string like "{123, 456}". Gets trimmed of {}.
         * @return  A vector of the EdgeIds separately.
         * @throw   std::invalid_argument
         * @throw   std::out_of_range
         */
        static std::vector<EdgeIdType>
                            parseEdgeIdsString(std::string& rEdgeIds)
        {
            boost::trim_if(rEdgeIds, boost::is_any_of("{}"));
            std::vector<std::string> idStrings;
            boost::split(idStrings, rEdgeIds, boost::is_any_of(","));

            std::vector<EdgeIdType> edgeIds;
            for(const std::string& idStr : idStrings)
            {
                edgeIds.push_back(Edge::parse(idStr));
            }
            return edgeIds;
        }

        /** Find the Edge that matches the OsmId in turning restriction.
         * @param   osmId       The original edge osm id.
         * @param   rEdgeIds    The cancidate edges that are near restriction.
         * @param   rTopology   The Topology to ask for edges.
         */
        static Edge&        findEdgeMatchingOsmId(
            OsmIdType                       osmId,
            const std::vector<EdgeIdType>&  rEdgeIds,
            const Topology&                 rTopology)
        {
            EdgeIdType edgeId = Edge::MAX_ID;
            for(EdgeIdType id : rEdgeIds)
            {
                Edge& edge = rTopology.getEdge(id);
                if(edge.osmId() == osmId)
                {
                    return edge;
                }
            }
            throw MapProviderException(
                "PostGisRestrictionQueries:Result:findEdgeMatchingOsmId: "
                "No edges matching osm_id: " + std::to_string(osmId));
        }

//        /** Find the common vertex that all edges must share, (only checking
//         * first and last edges though) or else it is not a valid vertex.
//         * @param   rEdgeIds    The Edges round a turning restriction.
//         * @param   rTopology   The topology where those edges must be in.
//         * @return  The vertexId of the common vertex.
//         * @throw   TopologyException if edges are not in topology.
//         * @throw   MapProviderException if edge list too small or
//         *          no common vertex.
//         */
//        static VertexIdType findCommonVertexId(
//            const std::vector<EdgeIdType>&  rEdgeIds,
//            const Topology&                 rTopology)
//        {
//            if(rEdgeIds.size() > 1)
//            {
//                const Edge& edge1 = rTopology.getEdge(rEdgeIds.at(0));
//                const Edge& edge2 =
//                    rTopology.getEdge(rEdgeIds.at(rEdgeIds.size()-1));
//                if(edge1.source() == edge2.target()
//                || edge1.source() == edge2.source())
//                {
//                    return edge1.source();
//                }
//                if(edge1.target() == edge2.target()
//                || edge1.target() == edge2.source())
//                {
//                    return edge1.target();
//                }
//            }
//            throw MapProviderException(
//                "PostGisRestrictionQueries:Result:findCommonVertexId: "
//                "No edges with common vertex.");
//        }
    };

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
