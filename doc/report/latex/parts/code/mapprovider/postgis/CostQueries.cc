/*
 * CostQueries.cc
 *
 * @author  Jonas Bergman
 */

#include "CostQueries.h"  // class implemented

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
void
CostQueries::getTravelTimeEdgeCosts(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rTopoEdgeTable,
    const std::string&      rOsmEdgeTable,
    const std::string&      rSchemaName)
{
    std::vector<std::string> columns {
        "maxspeed",
        "surface"
    };

    rResult = rTrans.exec(
        startOfQuery(rTopoEdgeTable) +
        queryColumns(columns) +
        midOfQuery(rSchemaName, rOsmEdgeTable) +
//        notNullColumns(columns) +
        endOfQuery()
    );
}

// static
void
CostQueries::addTravelTimeCosts(
    const pqxx::result&     rResult,
    Topology&               rTopology,
    const Configuration&    rConfig)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[TravelTimeCostResult::EDGE_ID].as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);

            Speed speed =
                row[TravelTimeCostResult::MAXSPEED].as<Speed>(
                    EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
            std::string surface_string =
                row[TravelTimeCostResult::SURFACE].as<std::string>("");

            addTravelTimeCostToEdge(edge, speed, surface_string, rConfig);
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addTravelTimeCost: ") + e.what());
    }
}

//static
void
CostQueries::addTravelTimeCostToEdge(
    Edge&                   rEdge,
    Speed                   speed,
    std::string&            surfaceString,
    const Configuration&    rConfig)
{
    bool hasMaxSpeed =
        (speed != EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
    bool hasSurface = surfaceString.length() > 0;
    if(!(hasMaxSpeed || hasSurface))
    {
        speed = getDefaultSpeedForEdge(rEdge, rConfig);
    }
    // look if surface restricts speed
    else if(hasSurface)
    {
        try
        {
            OsmHighway::SurfaceType surface =
                OsmHighway::parseSurfaceString(surfaceString);
            Speed surfaceSpeed =
                rConfig.getCostConfig().surfaceMaxSpeed.getSurfaceMaxSpeed(surface);
            if(surfaceSpeed < speed)
            {
                speed = surfaceSpeed;
            }
        }
        catch (OsmException& e)
        {
            throw MapProviderException(
                std::string("CostQueries:addTravelTime... ") +
                "could not parse surface " + surfaceString);
        }
    }
    double speed_mps = speed / 3.6;
    double travel_time = rEdge.geomData().length/ speed_mps;
    rEdge.edgeCost().addCost(EdgeCost::TRAVEL_TIME, travel_time);
    rEdge.setSpeed(speed);
}

// static
Speed
CostQueries::getDefaultSpeedForEdge(
    const Edge&             rEdge,
    const Configuration&    rConfig)
{
    OsmHighway::HighwayType type = rEdge.roadData().roadType;
    const CostConfig& costConfig = rConfig.getCostConfig();
    Speed speed=
        costConfig.defaultSpeed.getDefaultSpeed(type, CostConfig::DefaultSpeed::LOW);
    return speed;
}

// static
void
CostQueries::getOtherCosts(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rOsmPointTable,
    const std::string&      rTopoEdgeTable,
    const std::string&      rOsmEdgeTable,
    const std::string&      rSchemaName)
{
    rResult = rTrans.exec(
        "SELECT p.osm_id, "
        "       p.highway, "
        "       p.railway, "
        "       p.public_transport, "
        "       p.traffic_calming, "
        "       t.edge_id "
        "FROM  " + rOsmPointTable + " p, "
        "      " + rTopoEdgeTable + " t, "
        "      " + rOsmEdgeTable + " o, "
        "      " + rSchemaName + ".relation r "
        "WHERE  r.topogeo_id = (topo_geom).id "
        "AND    r.element_id = t.edge_id "
        "AND   (p.highway = 'bus_stop' OR "
        "       p.highway = 'crossing' OR "
        "       p.highway = 'give_way' OR"
        "       p.highway = 'mini_roundabout' OR"
        "       p.highway = 'stop' OR"
        "       p.highway = 'traffic_signals' OR"
        "       p.railway = 'level_crossing' OR"
        "       p.public_transport = 'stop_position' OR"
        "       p.traffic_calming = 'yes' OR"
        "       p.traffic_calming = 'bump' OR"
        "       p.traffic_calming = 'hump' OR"
        "       p.traffic_calming = 'table' OR"
        "       p.traffic_calming = 'cushion' OR"
        "       p.traffic_calming = 'rumble_strip' OR"
        "       p.traffic_calming = 'chicane' OR"
        "       p.traffic_calming = 'choker' OR"
        "       p.traffic_calming = 'island' "
        ")"
        "AND    ST_Intersects(p.way, t.geom) "
        "AND    o.highway IN " + OsmHighway::typesAsCommaSeparatedString()
    );
}

// static
void
CostQueries::addOtherCosts(
    const pqxx::result&     rResult,
    Topology&               rTopology,
    const Configuration&    rConfig)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[OtherCostResult::EDGE_ID].as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);

            std::string type_string = "highway=" +
                row[OtherCostResult::HIGHWAY].as<std::string>("");
            addOtherCostToEdge(edge, type_string, rConfig);

            type_string = "railway=" +
                row[OtherCostResult::RAILWAY].as<std::string>("");
            addOtherCostToEdge(edge, type_string, rConfig);

            type_string = "public_transport=" +
                row[OtherCostResult::PUBLIC_TRANSPORT].as<std::string>("");
            addOtherCostToEdge(edge, type_string, rConfig);

            type_string = "traffic_calming=" +
                row[OtherCostResult::TRAFFIC_CALMING].as<std::string>("");
            addOtherCostToEdge(edge, type_string, rConfig);
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("CostQueries:addOtherCosts..: ") + e.what());
    }
}

// static
void
CostQueries::addOtherCostToEdge(
    Edge&                   rEdge,
    const std::string&      key,
    const Configuration&    rConfig)
{
    size_t eq_char = key.find('=');
    if((eq_char == std::string::npos) || (eq_char == key.length() - 1))
    {
        return;
    }

    Cost cost = rConfig.getCostConfig().otherEdgeCosts.getOtherCost(key);
    rEdge.edgeCost().addCost(EdgeCost::OTHER, cost);
}

// static
void
CostQueries::addBarrierCostToEdge(
    Edge&                   rEdge,
    OsmBarrier::BarrierType type,
    const Configuration&    rConfig)
{
    if(rConfig.getBarrierCostsRule().costsToPass(type))
    {
        Cost cost = rConfig.getBarrierCostsRule().getCost(type);
        rEdge.edgeCost().addCost(EdgeCost::BARRIER, cost);
    }
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////
//static
std::string
CostQueries::startOfQuery(const std::string& rTopoEdgeTable)
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

//static
std::string
CostQueries::queryColumns(const std::vector<std::string>& rCols)
{
    std::ostringstream oss;
    for(const std::string& col : rCols)
    {
        oss << ", " << col;
    }
    return oss.str();
}

//static
std::string
CostQueries::midOfQuery(
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

//static
std::string
CostQueries::notNullColumns(const std::vector<std::string>& rCols)
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

//static
std::string
CostQueries::endOfQuery()
{
    return (
        ") AS osm "
        "ON edge_id = element_id "
        "ORDER BY edge_id ASC;"
    );
}

