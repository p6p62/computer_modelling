#pragma once
#include <functional>

class MarsagliaBrayGenerator
{
private:
	const double _math_expectation;
	const double _variance;
	const double _mean_square_deviation;
	const std::function<double()> _uniform_number_generator;
	std::vector<double> _buffer;

public:
	MarsagliaBrayGenerator(double math_expectation, double variance, const std::function<double()> uniform_number_generator);

public:
	double next();
};

