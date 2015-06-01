/*
 * TopologyQueries.cc
 *
 *  Created on: 2015-06-01
 *      Author: Jonas Bergman
 */

#include "TopologyQueries.h"  // class implemented


/////////////////////////////// TYPES  ///////////////////////////////////////
// Result---------------------------------------------------------------------


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
pqxx::result
TopologyQueries::getTopologyVertices(
    pqxx::transaction_base& rTrans,
    const std::string&      rVertexTable)
{
		return rTrans.exec(
				"SELECT node_id, ST_X(geom) AS x, ST_Y(geom) AS y "
				" FROM " + rVertexTable +
				" ORDER BY node_id ASC;"
		);
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////




