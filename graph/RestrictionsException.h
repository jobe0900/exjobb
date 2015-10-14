/**  Exception thrown by the Restrictions.
 *
 * #include "RestrictionsException.h"
 *
 * @author  Jonas Bergman
 */

#ifndef GRAPH_RESTRICTIONSEXCEPTION_H_
#define GRAPH_RESTRICTIONSEXCEPTION_H_

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
 * Exception to throw from Restrictions class.
 * More information of the type of exception is given in the 'what()' message.
 */
class RestrictionsException : public std::exception
{
public:
// LIFECYCLE
    /** Default constructor.
     */
    RestrictionsException() = delete;

    /** Constructor taking a message to display.
     *
     * @param   message     The message to prepend when 'what()' is called.
     */
    RestrictionsException(const std::string& rMessage) noexcept
        : std::exception(), mMessage(rMessage)
    {}

// OPERATORS
    void    addEdgeId(std::string edgeIdString) { mMessage += edgeIdString; }
// OPERATIONS
// ACCESS
// INQUIRY
    const char* what() const noexcept
    { return mMessage.c_str(); }

protected:
private:
// ATTRIBUTES
    std::string     mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_RESTRICTIONSEXCEPTION_H_ */
