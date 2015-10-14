/**  Exception thrown by the Topology package.
 *
 * #include "TopologyException.h"
 *
 * @author  Jonas Bergman
 */

#ifndef GRAPH_TOPOLOGYEXCEPTION_H_
#define GRAPH_TOPOLOGYEXCEPTION_H_

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
 * Exception to throw from the 'topology' package.
 * More information of the type of exception is given in the 'what()' message.
 */
class TopologyException : public std::exception
{
public:
// LIFECYCLE
    /** Default constructor.
     */
    TopologyException() = delete;

    /** Constructor taking a message to display.
     *
     * @param    message        The message to prepend when 'what()' is called.
     */
    TopologyException(const std::string& rMessage) noexcept
        : std::exception(), mMessage(rMessage)
    {}

// OPERATORS
// OPERATIONS
// ACCESS
// INQUIRY
    const char* what() const noexcept
    { return mMessage.c_str(); }

protected:
private:
// ATTRIBUTES
    std::string        mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOLOGYEXCEPTION_H_ */
