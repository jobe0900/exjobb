/**  Access to Access data from the OSM file.
 *
 * #include "OsmAccess.h"
 *
 * @author Jonas Bergman
 */

#ifndef OSM_OSMACCESS_H_
#define OSM_OSMACCESS_H_

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


class OsmAccess
{
public:
// TYPES
    enum AccessType
    {
        YES,
        PRIVATE,
        NO,
        PERMISSIVE,
        AGRICULTURAL,
        USE_SIDEPATH,
        DELIVERY,
        DESIGNATED,
        DISMOUNT,
        DISCOURAGED,
        FORESTRY,
        DESTINATION,
        CUSTOMERS,

        NR_ACCESS_TYPES
    };

// LIFECYCLE
    OsmAccess() = delete;
    OsmAccess(AccessType type);
    OsmAccess(const OsmAccess& from) = default;
    ~OsmAccess() = default;

// OPERATORS
// OPERATIONS
    /** Attempt to parse a string to a AccessType
     * @param   rTypeString     String which could contain a Access type
     * @return  A valid AccessType
     * @throw   OsmException if invalid string.
     */
    static AccessType  parseString(const std::string& rTypeString);

    /** Convert a Access Type to a string representation.
     * @param   accessType     The type to convert.
     * @return  string representation of the type.
     * @throw   OsmException if unknown vehicle type (out of bounds).
     */
    static std::string  toString(AccessType accessType);

    /** Convert this AccessType to a string.
     * @return  string representation of this VehicleType.
     */
    std::string         toString() const;

// ACCESS
// INQUIRY
protected:
private:
    AccessType               mType {YES};
    static const std::string sTypeStrings[];
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_OSMACCESS_H_ */
