#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	point3 center;
	double radius;
    shared_ptr<material> mat_ptr;

    sphere() {}
	sphere(point3 center, double r) : center(center), radius(r) {};
    sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    bool hit = false;

    vec3 oc = r.origin() - center; // Make ray from the center of the sphere

    // Get coefficients of equation (use b=2h to simplify equation)
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    // Calculate and check discriminant for hit
    auto discriminant = half_b * half_b - a * c;
    if (discriminant >= 0.0)
    {
        auto sqrtd = sqrt(discriminant);

        // Calculate roots

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) 
        {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
            {
                return hit; // Hit fails
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        
        vec3 outward_normal = (rec.p - center) / radius; // Calculate outward normal
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        hit = true;
    }

    return hit;
}

#endif // !SPHERE_H
