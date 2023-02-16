#include "MathFunctions.h"

std::vector<double> MathFunctions::separate_on_parts(unsigned int parts_count, double a, double b)
{
	++parts_count;
	std::vector<double> parts(parts_count);
	const double step{ (b - a) / (parts_count - 1) };
	double temp{ b };
	while (--parts_count)
	{
		parts[parts_count] = temp;
		temp -= step;
	}
	parts[0] = a;
	return parts;
}
