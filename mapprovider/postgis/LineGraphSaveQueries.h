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

    static void dropCreateSchema(
        pqxx::transaction_base& rTrans,
        const std::string& rSchemaName);

// ACCESS
// INQUIRY

protected:
private:
};

#endif /* MAPPROVIDER_POSTGIS_LINEGRAPHSAVEQUERIES_H_ */
