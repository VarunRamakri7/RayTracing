#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{
	public:
		point3 origin;
		point3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;

		camera()
		{
			auto aspect_ratio = 16.0 / 9.0;

			auto viewport_height = 2.0;
			auto viewport_width = aspect_ratio * viewport_height;

			auto focal_length = 1.0;

			origin = point3(0.0, 0.0, 0.0);
			horizontal = vec3(viewport_width, 0.0, 0.0);
			vertical = vec3(0.0, viewport_height, 0.0);
			lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0.0, 0.0, focal_length);
		}

		/// <summary>
		/// Construct a camera with the given vertical fov and aspect ratio
		/// </summary>
		/// <param name="fov">Vertical fov</param>
		/// <param name="aspect_ratio">Aspect ratio</param>
		camera(double fov, double aspect_ratio)
		{
			auto theta = degrees_to_radians(fov);
			auto h = tan(theta / 2.0f);
			
			auto viewport_height = 2.0f * h;
			auto viewport_width = aspect_ratio * viewport_height;

			auto focal_length = 1.0;

			origin = point3(0.0, 0.0, 0.0);
			horizontal = vec3(viewport_width, 0.0, 0.0);
			vertical = vec3(0.0, viewport_height, 0.0);
			lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0.0, 0.0, focal_length);
		}

		ray get_ray(double u, double v) const
		{
			return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
		}
};

#endif // !CAMERA_H
