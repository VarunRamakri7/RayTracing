#include<iostream>
#include <fstream>
#include <sstream>

#include "ray.h"
#include "color.h"

/// <summary>
/// Returns the root if the given ray hits a sphere of the given radius and center
/// </summary>
/// <param name="center">Center of the sphere</param>
/// <param name="radius">Radius of the sphere</param>
/// <param name="r">Ray</param>
/// <returns>Root of the sphere intersection</returns>
double hit_sphere(const point3& center, double radius, const ray& r)
{
	vec3 oc = r.origin() - center; // Make ray from the center of the sphere in the direction of the ray

	// Get coefficients of equation (use b=2h to simplify equation)
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	
	auto discriminant = half_b * half_b - a * c;
	
	// Return root depending on discriminant
	if (discriminant < 0.0)
	{
		return -1.0f;
	}
	else
	{
		return (-half_b - sqrt(discriminant)) / a;
	}
}

/// <summary>
/// Linearly blend white and blue depending on the y coordinate if ray does not hit sphere
/// </summary>
/// <param name="r">Ray</param>
/// <returns>Color of the ray</returns>
vec3 ray_color(const ray& r)
{
	auto t = hit_sphere(point3(0.0, 0.0, -1), 0.5, r); // Get roots of sphere-ray intersection

	// Check if the ray hits the sphere
	if(t > 0.0)
	{
		vec3 N = unit_vector(r.at(t) - vec3(0.0, 0.0, -1.0));
		return 0.5 * color(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0);
	}
	else
	{
		vec3 unit_dir = unit_vector(r.direction());
		t = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	}
}

/// <summary>
/// Write a ray traced colors into a .ppm file
/// </summary>
/// <returns></returns>
int main()
{
	std::fstream file("image.ppm");

	// Image
	const auto aspect = 16.0 / 9.0;
	int width = 400;
	int height = static_cast<int>(width / aspect);

	// Camera
	auto viewport_height = 2.0;
	auto viewport_width = aspect * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0.0, 0.0, 0.0);
	auto horizontal = vec3(viewport_width, 0.0, 0.0);
	auto vertical = vec3(0.0, viewport_height, 0.0);
	auto lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0.0, 0.0, focal_length);

	// Render
	//std::stringstream ss;
	file << "P3" << std::endl << width << " " << height << "\n255" << std::endl;

	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			auto u = double(i) / (width - 1);
			auto v = double(j) / (height - 1);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);
			write_color(file, pixel_color);

			//ss << pixel_color[0] << " " << pixel_color[1] << " " << pixel_color[2] << std::endl;
		}
	}

	//file << ss.str() << std::endl;

	file.close();

	std::cout << "End" << std::endl;

	return 0;
}