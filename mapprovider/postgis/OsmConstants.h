/*
 * OsmConstants.h
 *
 *  Created on: 2015-05-24
 *      Author: Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_OSMCONSTANTS_H_
#define MAPPROVIDER_POSTGIS_OSMCONSTANTS_H_


/**
 * To query the database for edge related data we fetch the following columns.
 */
struct EdgeQueryResult
{
    enum Columns
    {
        EDGE_ID,
        START_NODE,
        END_NODE,
        EDGE_LENGTH,
        CENTER_X,
        CENTER_Y,
        SOURCE_BEARING,
        TARGET_BEARING,
        OSM_ID,
        ELEMENT_ID,     // NOT USED: same as EDGE_ID_COL

        // road data
        HIGHWAY,
        JUNCTION,
        LANES,
        ONEWAY,

        // vehicle property

        // access
        ACCESS,
        MOTORCAR,
        GOODS,
        HGV,
        PSV,
        LHV,
        MOTOR_VEHICLE,
        VEHICLE,
    };
};

struct VehiclePropertiesQueryResult
{
    enum Columns
    {
        EDGE_ID,
        MAXHEIGHT,
        MAXLENGTH,
        MAXSPEED,
        MAXWEIGHT,
        MAXWIDTH,
        MINSPEED,

    };
};



#endif /* MAPPROVIDER_POSTGIS_OSMCONSTANTS_H_ */
