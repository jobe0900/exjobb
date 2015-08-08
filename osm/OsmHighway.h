/**  Access to Highway data from the OSM file.
 *
 * #include "OsmHighway.h" <BR>
 * -llib
 *
 * A longer description.
 *
 * @see something
 */

#ifndef OSM_OSMHIGHWAY_H_
#define OSM_OSMHIGHWAY_H_

// SYSTEM INCLUDES
//
#include <string>
#include <sstream>
#include <vector>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "OsmException.h"

// FORWARD REFERENCES
//
typedef unsigned Speed;


class OsmHighway
{
public:
// TYPES
    enum HighwayType
    {
        // roads
        MOTORWAY,
        MOTORWAY_LINK,
        TRUNK,
        TRUNK_LINK,
        PRIMARY,
        PRIMARY_LINK,
        SECONDARY,
        SECONDARY_LINK,
        TERTIARY,
        TERTIARY_LINK,
        UNCLASSIFIED,
        RESIDENTIAL,
        SERVICE,

        // special types
        LIVING_STREET,
        BUS_GUIDEWAY,
        ROAD,

        NR_HIGHWAY_TYPES
    };

    enum SurfaceType
    {
        PAVED,
        ASPHALT,
        COBBLESTONE,
        COBBLESTONE_FLATTENED,
        SETT,
        CONCRETE,
        CONCRETE_LANES,
        CONCRETE_PLATES,
        PAVING_STONES,
        METAL,
        WOOD,

        UNPAVED,
        COMPACTED,
        DIRT,
        EARTH,
        FINE_GRAVEL,
        GRASS,
        GRASS_PAVER,
        GRAVEL,
        GROUND,
        ICE,
        MUD,
        PEBBLESTONE,
        SALT,
        SAND,
        SNOW,
        WOODCHIPS,

        METAL_GRID,

        NR_SURFACE_TYPES
    };

    enum JunctionType
    {
        ROUNDABOUT
    };
    static constexpr const char* JUNCTION_ROUNDABOUT = "roundabout";

// LIFECYCLE
    OsmHighway() = delete;
    OsmHighway(HighwayType type);
    OsmHighway(const OsmHighway& from) = default;
    ~OsmHighway() = default;

// OPERATORS
// OPERATIONS
    /** Attempt to parse a string to a HighwayType
     * @param   rTypeString     String which could contain a Highway type
     * @return  A valid HighwayType
     * @throw   OsmException if invalid string.
     */
    static HighwayType  parseString(const std::string& rTypeString);

    /** Attempt to parse a string to a SurfaceType
     * @param   rTypeString     String which could contain a Surface type
     * @return  A valid SurfaceType
     * @throw   OsmException if invalid string.
     */
    static SurfaceType parseSurfaceString(const std::string& rSurfaceString);

    /** Convert a Highway Type to a string representation.
     * @param   highwayType     The type to convert.
     * @return  string representation of the type.
     * @throw   OsmException if unknown highway type (out of bounds).
     */
    static std::string  toString(HighwayType highwayType);

    /** Convert a SurfaceType to a string representation.
     * @param   surfaceType     The type to convert.
     * @return  string representation of the type.
     * @throw   OsmException if unknown highway type (out of bounds).
     */
    static std::string  toSurfaceString(SurfaceType surfaceType);

    /** Convert this HighwayType to a string.
     * @return  string representation of this HighwayType.
     */
    std::string         toString() const;

// ACCESS
    /**
     * @return A vector of all types as strings.
     */
    static const std::vector<std::string>& typeStrings();

    /**
     * @return A vector of all surface types as strings.
     */
    static const std::vector<std::string>& surfaceTypeStrings();

    /** Return "(motorway, trunk....)".
     * @return A string of all types, comma separated, with parentheses round.
     */
    static std::string typesAsCommaSeparatedString();

// INQUIRY
protected:
private:
    HighwayType                             mType {ROAD};
    static const std::vector<std::string>   sTypeStrings;
    static const std::vector<std::string>   sSurfaceTypeStrings;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_OSMHIGHWAY_H_ */
