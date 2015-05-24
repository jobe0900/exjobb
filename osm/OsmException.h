/**  Exception thrown in the 'osm' package.
 *
 * #include "OsmException.h"
 *
 * @author Jonas Bergman
 */

#ifndef OSM_OSMEXCEPTION_H_
#define OSM_OSMEXCEPTION_H_

// SYSTEM INCLUDES
//
#include <exception>
#include <string>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//

// FORWARD REFERENCES
//

/**
 * Exception to throw in the 'osm' package.
 * More information of the type of exception is given in the 'what()' message.
 */
class OsmException : public std::exception
{
public:
// LIFECYCLE
    /** Default constructor.
     */
    OsmException() = delete;

    /** Constructor taking a message to display.
     *
     * @param   message     The message to prepend when 'what()' is called.
     */
    OsmException(const std::string& rMessage) noexcept
        : std::exception(), mMessage(rMessage)
    {}

// OPERATORS
// OPERATIONS
// ACCESS
// INQUIRY
    const char* what() const noexcept
    { return (mMessage.c_str()); }

protected:
private:
// ATTRIBUTES
    std::string     mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_OSMEXCEPTION_H_ */
