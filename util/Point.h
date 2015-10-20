/**  Data structure for Point.
 *
 * #include "Point.h"
 *
 * @author  Jonas Bergman
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
// ATTRIBUTES
    double    x {0.0};
    double    y {0.0};

    Point(double x, double y) : x(x), y(y) {}
    Point() = default;
    Point(const Point&) = default;

// OPERATORS
    friend std::ostream& operator<<(std::ostream& os, const Point& rPoint)
    {
        os << std::fixed << "Point [x: " << rPoint.x << ", y: " << rPoint.y << "]";
        return os;
    }

    bool    operator==(const Point& rhs) const
    {
        return (rhs.x == x) && (rhs.y == y);
    }
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* UTIL_POINT_H_ */
