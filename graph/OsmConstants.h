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
        EDGE_ID,
        START_NODE,
        END_NODE,
        EDGE_LENGTH,
        CENTER_X,
        CENTER_Y,
        SOURCE_BEARING,
        TARGET_BEARING,
        OSM_ID,
        ELEMENT_ID,     // NOT USED: same as EDGE_ID
        // road data
        HIGHWAY,
        JUNCTION,
        LANES,
        ONEWAY,
        // vehicle property
        MAXHEIGHT,
        MAXLENGTH,
        MAXSPEED,       // NOT USED YET
        MAXWEIGHT,
        MAXWIDTH,
        MINSPEED,       // NOT USED YET
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
};

#endif /* GRAPH_OSMCONSTANTS_H_ */
