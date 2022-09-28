#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "rtweekend.h"

using std::sqrt;
using std::fabs;

class vec3
{
public:
    double e[3];

    vec3() : e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) 
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) 
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) 
    {
        return *this *= 1 / t;
    }

    double length() const 
    {
        return sqrt(length_squared());
    }

    double length_squared() const 
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool near_zero() const 
    {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    inline static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};


// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream& out, const vec3& v) 
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) 
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) 
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) 
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) 
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) 
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t) 
{
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) 
{
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) 
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

/// <summary>
/// Get a random point inside a unit sphere
/// </summary>
/// <returns></returns>
vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

/// <summary>
/// Pick random points on the unit sphere (offset along the surface normal)
/// For True Lambertian Reflection
/// </summary>
/// <returns>Random point on unit sphere along the surface normal</returns>
vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

/// <summary>
/// Uniformly scatter direction for all angles away from the hit point,
/// with no dependence on the angle from the normal
/// </summary>
/// <param name="normal">Object's normal</param>
/// <returns>Point in the same hemisphere as the normal</returns>
vec3 random_in_hemisphere(const vec3& normal)
{
    vec3 in_unit_sphere = random_in_unit_sphere();
    
    // Check if point is in the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

/// <summary>
/// Reflect a vector along a normal
/// </summary>
/// <param name="v">Vector to reflect</param>
/// <param name="n">Normal to reflect along</param>
/// <returns>Reflection of v on n</returns>
vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

/// <summary>
/// Refract the vector along a normal between two mediums
/// with indices of refractions as etai and etat.
/// </summary>
/// <param name="uv">Incident vector</param>
/// <param name="n">Normal to surface</param>
/// <param name="etai_by_etat">Division of indices of refraction</param>
/// <returns></returns>
vec3 refract(const vec3& uv, const vec3& n, double etai_by_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0); // Get angle between ray and normal
    
    vec3 r_out_perp = etai_by_etat * (uv + cos_theta * n); // Parallel out ray
    vec3 r_out_par = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n; // Perpendicular out ray

    return r_out_perp + r_out_par; // Refracted ray
}

#endif