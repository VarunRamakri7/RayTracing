#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material
{
    public:
        /// <summary>
        /// Produce a scattered or absorbed ray.
        /// If scattered, say how much attenuation is needed for the ray
        /// </summary>
        /// <param name="r_in">Incident ray</param>
        /// <param name="rec">Hit record</param>
        /// <param name="attenuation">Amount of attenuation</param>
        /// <param name="scattered">Scattered ray</param>
        /// <returns>True if scattered, false if miss</returns>
        virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material
{
    public:
        color albedo;

        lambertian(const color& a) : albedo(a) {}

        /// <summary>
        /// Scatter ray when it hits a lambertian surface
        /// </summary>
        /// <param name="r_in">Incident ray</param>
        /// <param name="rec">Hit record</param>
        /// <param name="attenuation"></param>
        /// <param name="scattered"></param>
        /// <returns>True if scattered, false if miss</returns>
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override
        {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Check for degenerate scatter direction
            if (scatter_direction.near_zero())
            {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material
{
    public:
        color albedo;
        double fuzz; // Fuzziness quotient for material

        metal(const color& a) : albedo(a) {}
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        /// <summary>
        /// Scatter ray when it hits a metal surface
        /// </summary>
        /// <param name="r_in">Incident ray</param>
        /// <param name="rec">Hit record</param>
        /// <param name="attenuation"></param>
        /// <param name="scattered"></param>
        /// <returns>True if scattered, false if miss</returns>
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override
        {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere()); // Check scattering with fuzziness
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        };
};

class dielectric : public material
{
    public:
        double ir; // Index of refraction

        dielectric(double refr_index) : ir(refr_index) {}

        /// <summary>
        /// Scatter ray when it hits a dielectric surface
        /// </summary>
        /// <param name="r_in">Incident ray</param>
        /// <param name="rec">Hit record</param>
        /// <param name="attenuation"></param>
        /// <param name="scattered"></param>
        /// <returns>True if scattered, false if miss</returns>
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override
        {
            attenuation = color(1.0, 1.0, 1.0);

            double ratio_refr = rec.front_face ? (1.0 / ir) : ir; // Set ratio of refraction

            vec3 unit_dir = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_dir, rec.normal, ratio_refr);

            scattered = ray(rec.p, refracted);

            return true;
        }
};
#endif