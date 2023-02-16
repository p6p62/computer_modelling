#include "MathFunctions.h"
#include <math.h>

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

double MathFunctions::moment_initial(const std::vector<double>& data, int moment_number)
{
	if (data.empty())
		return 0;
	double sum{ 0 };
	for (double v : data)
		sum += pow(v, moment_number);
	return sum / data.size();
}

double MathFunctions::moment_central(const std::vector<double>& data, int moment_number, const double* math_expectation)
{
	double avg{ math_expectation ? *math_expectation : MathFunctions::math_expectation(data) };
	return moment_initial(data, moment_number) - pow(avg, moment_number);
}

double MathFunctions::math_expectation(const std::vector<double>& data)
{
	if (data.empty())
		return 0;
	double avg{ 0 };
	for (double v : data)
		avg += v;
	return avg / data.size();
}

double MathFunctions::variance(const std::vector<double>& data, const double* math_expectation)
{
	return moment_central(data, 2, math_expectation);
}
