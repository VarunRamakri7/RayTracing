#include <iostream>
#include <fstream>
#include <sstream>

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

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
/// Set the color of the ray given a world of hittable objects
/// </summary>
/// <param name="r">Ray</param>
/// <param name="world">Hittable objects</param>
/// <returns>Color</returns>
color ray_color(const ray& r, const hittable& world)
{
	color col;

	// Check if the ray hits the world
	hit_record rec;
	if (world.hit(r, 0, infinity, rec))
	{
		col = 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
	}
	else
	{
		vec3 unit_dir = unit_vector(r.direction());
		auto t = 0.5 * (unit_dir.y() + 1.0);

		col = (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	}

	return col;
}

/// <summary>
/// Write a ray traced scene into a .ppm file
/// </summary>
/// <returns></returns>
int main()
{
	std::fstream file("image.ppm");

	// Image
	const auto aspect = 16.0 / 9.0;
	int width = 400;
	int height = static_cast<int>(width / aspect);

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100));

	// Camera
	auto viewport_height = 2.0;
	auto viewport_width = aspect * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0.0, 0.0, 0.0);
	auto horizontal = vec3(viewport_width, 0.0, 0.0);
	auto vertical = vec3(0.0, viewport_height, 0.0);
	auto lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0.0, 0.0, focal_length);

	// Render
	file << "P3" << std::endl << width << " " << height << "\n255" << std::endl;

	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			auto u = double(i) / (width - 1);
			auto v = double(j) / (height - 1);

			// Find color of pixel
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			color pixel_color = ray_color(r, world);

			write_color(file, pixel_color); // Write color into file
		}
	}

	file.close();

	std::cout << "End" << std::endl;

	return 0;
}