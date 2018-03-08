#ifndef CONST_H_
#define CONST_H_

#include <math.h>

#define ENABLE_GRID

// Math constants
#define PI 3.141592653589

// Configuration constants
// The max ray trace depth, stop ray trace procedure once depth exceeds it
#define MAX_TRACE_DEPTH 6

#define GRID_SIZE 16

enum {
    INTERSECT_HIT = 0,
    INTERSECT_MISS = 1,
    INTERSECT_IN = 2,
};

#define EPSILON 1e-5
// If two double value are equal
#define ALMOST_EQUAL(a, b) (fabs((a) - (b)) < EPSILON)

#endif // CONST_H_