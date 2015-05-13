/**  Exception thrown by the Database package.
 *
 * #include "DatabaseException.h"
 *
 * A longer description.
 *
 * @see something
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_DATABASE_DATABASEEXCEPTION_H_
#define MAPPROVIDER_DATABASE_DATABASEEXCEPTION_H_

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
 * Exception to throw from the 'database' package.
 * More information of the type of exception is given in the 'what()' message.
 */
class DatabaseException : public std::exception
{
public:
// LIFECYCLE

	/** Default constructor.
	 */
	DatabaseException() = delete;

	/** Constructor taking a message to display.
	 *
	 * @param	message		The message to prepend when 'what()' is called.
	 */
	DatabaseException(const std::string& rMessage) noexcept
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

#endif /* MAPPROVIDER_DATABASE_DATABASEEXCEPTION_H_ */
