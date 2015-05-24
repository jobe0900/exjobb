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

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "OsmException.h"

// FORWARD REFERENCES
//


class OsmHighway
{
public:
// TYPES
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

// LIFECYCLE
    OsmHighway() = default;
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

    /** Convert a Highway Type to a string representation.
     * @param   highwayType     The type to convert.
     * @return  string representation of the type.
     * @throw   OsmException if unknown highway type (out of bounds).
     */
    static std::string  toString(HighwayType highwayType);

// ACCESS
// INQUIRY
protected:
private:
    static const std::string typeStrings[];
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_OSMHIGHWAY_H_ */
