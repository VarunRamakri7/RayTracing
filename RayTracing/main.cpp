#include<iostream>
#include <fstream>
#include <sstream>

#include "vec3.h"

int main()
{
	std::ofstream file("image.ppm");

	int nx = 200;
	int ny = 100;

	std::stringstream ss;
	ss << "P3" << std::endl << nx << " " << ny << "\n255" << std::endl;

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2f;
			
			vec3 color(r, g, b);

			int ir = int(255.99 * color[0]);
			int ig = int(255.99 * color[1]);
			int ib = int(255.99 * color[2]);

			ss << ir << " " << ig << " " << ib << std::endl;
		}
	}

	file << ss.str() << std::endl;

	file.close();

	std::cout << "End" << std::endl;

	return 0;
}