/**  Exception thrown by the 'lgu' package.
 *
 * #include "LineGraphUtilityException.h"
 *
 * @author Jonas Bergman
 */

#ifndef LGU_LINEGRAPHUTILITYEXCEPTION_H_
#define LGU_LINEGRAPHUTILITYEXCEPTION_H_

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
 * Exception to throw from the 'graph' package.
 * More information of the type of exception is given in the 'what()' message.
 */
class LineGraphUtilityException : public std::exception
{
public:
// LIFECYCLE
    /** Default constructor.
     */
    LineGraphUtilityException() = delete;

    /** Constructor taking a message to display.
     *
     * @param   message     The message to prepend when 'what()' is called.
     */
    LineGraphUtilityException(const std::string& rMessage) noexcept
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

#endif /* LGU_LINEGRAPHUTILITYEXCEPTION_H_ */
