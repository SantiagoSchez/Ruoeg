//
// A struct representing a point with a direction
//

#ifndef RUOEG_UTILS_POINT_H_
#define RUOEG_UTILS_POINT_H_

#include "Direction.h"

struct Point
{
	Point() : x(0), y(0), dir(Direction::None), x_mod(0), y_mod(0) {};
	Point(int x_, int y_) : x(x_), y(y_) {};
	int x;
	int y;
	Direction dir;
	// Variables used to modify the prior ones when building a 
	// connection between features i.e. a door/lit.
	int x_mod;
	int y_mod;
};

#endif // RUOEG_UTILS_POINT_H_