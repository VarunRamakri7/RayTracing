#include <iostream>
#include <fstream>
#include <sstream>

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

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
/// Recursively set the color of the ray given a world of hittable objects
/// </summary>
/// <param name="r">Ray</param>
/// <param name="world">Hittable objects</param>
/// <param name="depth">Maximum recursion depth</param>
/// <returns>Color</returns>
color ray_color(const ray& r, const hittable& world, int depth)
{
	color col;

	// Check if the ray hits the world
	hit_record rec;

	// Check if we've exceeded the ray bounce limit, no more light is gathered is we have
	if (depth <= 0)
	{
		col = color(0.0, 0.0, 0.0);
	}
	else
	{
		if (world.hit(r, 0, infinity, rec))
		{
			point3 target = rec.p + rec.normal + random_in_unit_sphere(); // Choose a random target point inside the sphere
			
			col = 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
		}
		else
		{
			vec3 unit_dir = unit_vector(r.direction());
			auto t = 0.5 * (unit_dir.y() + 1.0);

			col = (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
		}
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
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));

	// Camera
	camera cam;

	// Render
	file << "P3" << std::endl << width << " " << height << "\n255" << std::endl;

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			color pixel_color(0.0, 0.0, 0.0);

			for (int s = 0; s < samples_per_pixel; ++s)
			{
				auto u = (i + random_double()) / (width - 1);
				auto v = (j + random_double()) / (height - 1);

				ray r = cam.get_ray(u, v); // Shoot ray
				pixel_color += ray_color(r, world, max_depth); // Find color of pixel

			}

			write_color(file, pixel_color, samples_per_pixel); // Write color into file
		}
	}

	file.close();

	std::cout << "End" << std::endl;

	return 0;
}