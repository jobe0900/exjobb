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
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* UTIL_POINT_H_ */
