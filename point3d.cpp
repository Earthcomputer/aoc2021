
#include "point3d.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Point3d& point) {
    return os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
}
