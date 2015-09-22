/*
 * LineGraphSaveQueries.cc
 *
 *      Author: Jonas Bergman
 */

#include "LineGraphSaveQueries.h"

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
void
LineGraphSaveQueries::dropCreateSchema(
    pqxx::transaction_base& rTrans,
    const std::string&      rSchemaName)
{
    rTrans.exec(
            "DROP SCHEMA IF EXISTS " + rSchemaName + " CASCADE; "
            "CREATE SCHEMA " + rSchemaName
    );
}

//static
void
LineGraphSaveQueries::dropCreateLineTable(
    pqxx::transaction_base& rTrans,
    const std::string&      rTableName)
{
    rTrans.exec(
            "DROP TABLE IF EXISTS " + rTableName + " CASCADE; "
            "CREATE TABLE " + rTableName + " ( "
//            "    topo_id  bigint, "
            "    cost     double precision, "
            "    geom     geometry(LineString, 900913) "
            "); "
    );
}

//static
void
LineGraphSaveQueries::dropCreateNodeTable(
    pqxx::transaction_base& rTrans,
    const std::string&      rTableName)
{
    rTrans.exec(
            "DROP TABLE IF EXISTS " + rTableName + " CASCADE; "
            "CREATE TABLE " + rTableName + " ( "
            "    topo_id  bigint unique, "
            "    geom     geometry(Point, 900913) "
            "); "
    );
}

//static
void
LineGraphSaveQueries::insertNode(
    pqxx::transaction_base& rTrans,
    const std::string&      rTableName,
    EdgeIdType              id,
    const std::string&      rGeomString)
{
    std::string sql =
        "INSERT INTO " + rTableName +" (topo_id, geom) "
        "SELECT " + std::to_string(id) + ", ST_GeomFromText('" + rGeomString + "', 900913) "
        "WHERE NOT EXISTS ("
        "    SELECT topo_id FROM " + rTableName +
        "    WHERE topo_id = " + std::to_string(id) + " );";

    std::cerr << "SQL: " << sql << std::endl;

    rTrans.exec(
        sql
    );
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////

