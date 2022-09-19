#include<iostream>
#include <fstream>
#include <sstream>

#include "ray.h"

/// <summary>
/// Linearly blend white and blue depending on the y coordinate
/// </summary>
/// <param name="r">Ray</param>
/// <returns></returns>
vec3 lerp(const ray& r)
{
	vec3 unit_dir = unit_vector(r.direction()); // Make unit vector
	float t = 0.5f * (unit_dir.y() + 1.0f); // Change range from [-1, 1] to [0, 1]

	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	std::ofstream file("image.ppm");

	int nx = 200;
	int ny = 100;

	std::stringstream ss;
	ss << "P3" << std::endl << nx << " " << ny << "\n255" << std::endl;

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			
			vec3 col = lerp(r);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			ss << ir << " " << ig << " " << ib << std::endl;
		}
	}

	file << ss.str() << std::endl;

	file.close();

	std::cout << "End" << std::endl;

	return 0;
}