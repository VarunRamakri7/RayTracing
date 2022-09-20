#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

/// <summary>
/// Convert degrees to radians
/// </summary>
/// <param name="degrees">Angle in degrees</param>
/// <returns>Angle in radians</returns>
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

/// <summary>
/// Generate a random double in [0, 1]
/// </summary>
/// <returns></returns>
inline double random_double()
{
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

/// <summary>
/// Generate a random double in [min, max]
/// </summary>
/// <returns></returns>
inline double random_double(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

/// <summary>
/// Clamp a double between a minimum and a maximum
/// </summary>
/// <param name="x">Value to clamp</param>
/// <param name="min">Minimum value of clamp range</param>
/// <param name="max">Maximum value of clamp range</param>
/// <returns></returns>
inline double clamp(double x, double min, double max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif