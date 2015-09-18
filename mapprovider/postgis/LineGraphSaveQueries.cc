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
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////

