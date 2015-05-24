/*
 * OsmConstants.h
 *
 *  Created on: 23 maj 2015
 *      Author: jonas
 */

#ifndef GRAPH_OSMCONSTANTS_H_
#define GRAPH_OSMCONSTANTS_H_


namespace OsmConstants
{
    enum EdgeResultColumns
    {
        EDGE_ID_COL,
        START_NODE_COL,
        END_NODE_COL,
        EDGE_LENGTH_COL,
        CENTER_X_COL,
        CENTER_Y_COL,
        SOURCE_BEARING_COL,
        TARGET_BEARING_COL,
        OSM_ID_COL,
        ELEMENT_ID_COL,     // NOT USED: same as EDGE_ID_COL
        // road data
        HIGHWAY_COL,
        JUNCTION_COL,
        LANES_COL,
        ONEWAY_COL,
        // vehicle property
        MAXHEIGHT_COL,
        MAXLENGTH_COL,
        MAXSPEED_COL,       // NOT USED YET
        MAXWEIGHT_COL,
        MAXWIDTH_COL,
        MINSPEED_COL,       // NOT USED YET
        // access
        ACCESS_COL,
        MOTORCAR_COL,
        GOODS_COL,
        HGV_COL,
        PSV_COL,
        LHV_COL,
        MOTOR_VEHICLE_COL,
        VEHICLE_COL,
    };

    enum HighwayType
    {
        // roads
        MOTORWAY,
        TRUNK,
        PRIMARY,
        SECONDARY,
        TERTIARY,
        UNCLASSIFIED,
        RESIDENTIAL,
        SERVICE,

        // link roads
        MOTORWAY_LINK,
        TRUNK_LINK,
        PRIMARY_LINK,
        SECONDARY_LINK,
        TERTIARY_LINK,

        // special types
        LIVING_STREET,
        BUS_GUIDEWAY,
        ROAD,

        NR_HIGHWAY_TYPES
    };

    // road types
    static constexpr const char* HighwayTypeStrings[] =
    {
        "motorway",
        "trunk",
        "primary",
        "secondary",
        "tertiary",
        "unclassified",
        "residential",
        "service",

        "motorway_link",
        "trunk_link",
        "primary_link",
        "secondary_link",
        "tertiary_link",

        "living_street",
        "bus_guideway",
        "road",
    };

    enum VehicleType
    {
        VEHICLE_MOTORCAR,
        VEHICLE_GOODS,
        VEHICLE_HGV,
        VEHICLE_PSV,
        VEHICLE_LHV,
        VEHICLE_MOTOR_VEHICLE,
        VEHICLE_VEHICLE,

        NR_VEHICLE_TYPES
    };

    static constexpr const char* VehicleTypeStrings[] =
    {
        "motorcar",
        "goods",
        "hgv",
        "psv",
        "lhv",
        "motor_vehicle",
        "vehicle"
    };

    enum AccessType
    {
        ALLOWED,
        RESTRICTED,
        NOT_SPECIFIED
    };

    static constexpr const char* ROUNDABOUT = "roundabout";
    static constexpr const char* ACCESS = "access";
    static constexpr const char* ACCESS_YES = "yes";
    static constexpr const char* ACCESS_PERMISSIVE = "permissive";
    static constexpr const char* ACCESS_DESIGNATED = "designated";

};

#endif /* GRAPH_OSMCONSTANTS_H_ */
