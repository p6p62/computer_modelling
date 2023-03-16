#include "MarsagliaBrayGenerator.h"

MarsagliaBrayGenerator::MarsagliaBrayGenerator(double math_expectation, double variance, const std::function<double()>& uniform_number_generator) : _uniform_number_generator{ uniform_number_generator }, _math_expectation{ math_expectation }, _variance{ variance }
{
}

double MarsagliaBrayGenerator::next()
{
	double value1{ -1 + 2 * _uniform_number_generator() };
	double value2{ -1 + 2 * _uniform_number_generator() };
	// TODO
	return 0.0;
}
