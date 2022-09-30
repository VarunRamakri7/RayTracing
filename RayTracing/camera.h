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
		vec3 u, v, w; // Orthonormal basis vectors
		double lens_radius;

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

		/// <summary>
		/// Construct an arbitrary camera with a lookfrom and lookat point,
		/// and an up vector
		/// </summary>
		/// <param name="lookfrom">Point the camera is looking from</param>
		/// <param name="lookat">Point the camera is looking at</param>
		/// <param name="up">World up vector</param>
		/// <param name="fov">vertical fov</param>
		/// <param name="aspect">Aspect ratio</param>
		camera(point3 lookfrom, point3 lookat, vec3 up, double fov, double aspect_ratio)
		{
			auto theta = degrees_to_radians(fov);
			auto h = tan(theta / 2.0f);

			auto viewport_height = 2.0f * h;
			auto viewport_width = aspect_ratio * viewport_height;

			// Create orthonormal basis vectors to describe camera orientation
			auto w = unit_vector(lookfrom - lookat);
			auto u = unit_vector(cross(up, w));
			auto v = cross(w, u);

			origin = lookfrom;
			horizontal = viewport_width * u;
			vertical = viewport_height * v;
			lower_left_corner = origin - (horizontal / 2.0f) - (vertical / 2.0f) - w;
		}

		/// <summary>
		/// Construct an arbitrary camera with a lookfrom and lookat point,
		/// an up vector, aperture size, and focus distance
		/// </summary>
		/// <param name="lookfrom">Point the camera is looking from</param>
		/// <param name="lookat">Point the camera is looking at</param>
		/// <param name="up">World up vector</param>
		/// <param name="fov">vertical fov</param>
		/// <param name="aspect">Aspect ratio</param>
		/// <param name="aperture">Size of camera aperture</param>
		/// <param name="focus_dist">Distance from camera at which everything is in focus</param>
		camera(point3 lookfrom, point3 lookat, vec3 up, double fov, double aspect_ratio, double aperture, double focus_dist)
		{
			auto theta = degrees_to_radians(fov);
			auto h = tan(theta / 2.0f);

			auto viewport_height = 2.0f * h;
			auto viewport_width = aspect_ratio * viewport_height;

			w = unit_vector(lookfrom - lookat);
			u = unit_vector(cross(up, w));
			v = cross(w, u);

			origin = lookfrom;
			horizontal = focus_dist * viewport_width * u;
			vertical = focus_dist * viewport_height * v;
			lower_left_corner = origin - (horizontal / 2.0) - (vertical / 2.0) - (focus_dist * w);

			lens_radius = aperture / 2.0;
		}

		ray get_ray(double s, double t) const
		{
			vec3 rd = lens_radius * random_in_unit_disk();
			vec3 offset = (u * rd.x()) + (v * rd.y());

			return ray(origin + offset, lower_left_corner + (s * horizontal) + (t * vertical) - origin - offset);
		}
};

#endif // !CAMERA_H
