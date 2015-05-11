/**  Data structure for Point in Mercator, unit meters.
 *
 * #include "TopologyNode.h"
 *
 * A longer description.
 *
 * @see something
 */

#ifndef UTIL_POINT_H_
#define UTIL_POINT_H_

// SYSTEM INCLUDES
//
#include <ostream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//

// FORWARD REFERENCES
//

struct Point
{
	double	x;
	double	y;

	Point(double x, double y) : x(x), y(y) {}
	Point() = delete;
	Point(const Point&) = default;

// OPERATORS
	friend std::ostream& operator<<(std::ostream& os, const Point& rPoint)
	{
		os << std::fixed << "Point [x: " << rPoint.x << ", y: " << rPoint.y << "]";
		return os;
	}

	bool	operator==(const Point& rhs) const
	{
		return (rhs.x == x) && (rhs.y == y);
	}
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* UTIL_POINT_H_ */
