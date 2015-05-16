/**  Exception thrown by the Graph package.
 *
 * #include "GraphException.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_GRAPHEXCEPTION_H_
#define GRAPH_GRAPHEXCEPTION_H_

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
class GraphException : public std::exception
{
public:
// LIFECYCLE
	/** Default constructor.
	 */
	GraphException() = delete;

	/** Constructor taking a message to display.
	 *
	 * @param	message		The message to prepend when 'what()' is called.
	 */
	GraphException(const std::string& rMessage) noexcept
		: std::exception(), mMessage(rMessage)
	{}

// OPERATORS
// OPERATIONS
// ACCESS
// INQUIRY
	const char* what() const noexcept
	{return (mMessage + " " + std::exception::what()).c_str();}

protected:
private:
// ATTRIBUTES
	std::string		mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_GRAPHEXCEPTION_H_ */
