#include "MarsagliaBrayGenerator.h"

MarsagliaBrayGenerator::MarsagliaBrayGenerator(double math_expectation, double variance, const std::function<double()> uniform_number_generator) : _uniform_number_generator{ uniform_number_generator }, _math_expectation{ math_expectation }, _variance{ variance }, _mean_square_deviation{ sqrt(_variance) }
{
}

double MarsagliaBrayGenerator::next()
{
	double result;
	if (!_buffer.empty())
	{
		result = *_buffer.rbegin();
		_buffer.pop_back();
	}
	else
	{
		double uniform_value1{ -1 + 2 * _uniform_number_generator() };
		double uniform_value2{ -1 + 2 * _uniform_number_generator() };
		while (uniform_value1 * uniform_value1 + uniform_value2 * uniform_value2 > 1)
		{
			uniform_value1 = -1 + 2 * _uniform_number_generator();
			uniform_value2 = -1 + 2 * _uniform_number_generator();
		}

		double temp_square{ uniform_value1 * uniform_value1 + uniform_value2 * uniform_value2 };
		if (temp_square == 0)
			temp_square = 1e-3;
		if (uniform_value1 == 0)
			uniform_value1 = 1e-3;
		double normal_value1{ uniform_value1 * sqrt(-2 * log(temp_square) / temp_square) };
		double normal_value2{ normal_value1 * uniform_value2 / uniform_value1 };

		result = normal_value1;
		_buffer.push_back(normal_value2);
	}
	return _math_expectation + _mean_square_deviation * result;
}
