/** Queries for saving the LineGraph to database
 * #include "LineGraphSaveQueries.h"
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_LINEGRAPHSAVEQUERIES_H_
#define MAPPROVIDER_POSTGIS_LINEGRAPHSAVEQUERIES_H_

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
//#include "../../osm/OsmHighway.h"
//#include "../../graph/Edge.h"
//#include "../../graph/EdgeRestriction.h"
#include "../../graph/Topology.h"
#include "../../graph/Graph.h"
//#include "../../graph/Vertex.h"
//#include "../MapProviderException.h"

/** Class for holding static queries for saving the line graph to database
 */
class LineGraphSaveQueries
{
public:
// TYPES

// LIFECYCLE
    LineGraphSaveQueries() = delete;
    LineGraphSaveQueries(const LineGraphSaveQueries& from) = delete;
    ~LineGraphSaveQueries() = default;

// OPERATORS
// OPERATIONS

    /** Create a new schema, dropping any existing with the same name first.
     * @param   rTrans      Transaction to perform the query in.
     * @param   rSchemaName Name of the schema to create.
     * @throw   pqxx::pqxx_exception
     */
    static void dropCreateSchema(
        pqxx::transaction_base& rTrans,
        const std::string&      rSchemaName);

    /** Create a new table for lines, dropping any existing with the same name.
     * @param   rTrans          Transaction to perform the query in.
     * @param   rTableName      Name of the table to create.
     * @throw   pqxx::pqxx_exception
     */
    static void dropCreateLineTable(
        pqxx::transaction_base& rTrans,
        const std::string&      rTableName);

    /** Create a new table for nodes, dropping any existing with the same name.
     * @param   rTrans          Transaction to perform the query in.
     * @param   rTableName      Name of the table to create.
     * @throw   pqxx::pqxx_exception
     */
    static void dropCreateNodeTable(
        pqxx::transaction_base& rTrans,
        const std::string&      rTableName);

// ACCESS
// INQUIRY

protected:
private:
};

#endif /* MAPPROVIDER_POSTGIS_LINEGRAPHSAVEQUERIES_H_ */
