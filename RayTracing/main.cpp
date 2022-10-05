#include <iostream>
#include <fstream>
#include <sstream>

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

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
		// Check if ray hits target and prevent shadow acne
		if (world.hit(r, 0.001, infinity, rec))
		{
			// Metal material
			ray scattered;
			color attenuation;

			if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			{
				col = attenuation * ray_color(scattered, world, depth - 1);
			}
			else
			{
				col = color(0.0, 0.0, 0.0);
			}

			/*
			// Lambertial Material
			point3 target = rec.p + random_in_hemisphere(rec.normal); // Choose a random point on the sphere

			col = 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
			*/
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

hittable_list random_scene()
{
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, ground_material));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4.0, 0.2, 0.0)).length() > 0.9)
			{
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8)
				{
					// Diffuse material
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95)
				{
					// metal material
					auto albedo = color::random(0.5, 1.0);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else
				{
					// Glass material
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0.0, 1.0, 0.0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4.0, 1.0, 0.0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4.0, 1.0, 0.0), 1.0, material3));

	return world;
}

/// <summary>
/// Write a ray traced scene into a .ppm file
/// </summary>
/// <returns></returns>
int main()
{
	std::fstream file("image.ppm");

	// Image
	const auto aspect = 3.0 / 2.0;
	int width = 800;
	int height = static_cast<int>(width / aspect);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	// World
	auto world = random_scene();

	// Make materials for world
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	// Add objects with materials to world
	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera
	point3 lookfrom(13.0, 2.0, 3.0);
	point3 lookat(0.0, 0.0, 0.0);
	vec3 up(0.0, 1.0, 0.0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, up, 20.0, aspect, aperture, dist_to_focus);

	// Render
	file << "P3" << std::endl << width << " " << height << "\n255" << std::endl;

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			color pixel_color(0.0, 0.0, 0.0);

			// Antialiase image
			for (int s = 0; s < samples_per_pixel; ++s)
			{
				// Send rays through each sample of a pixel and then average them for the pixel
	
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