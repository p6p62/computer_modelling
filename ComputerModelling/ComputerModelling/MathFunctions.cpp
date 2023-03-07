#include "MathFunctions.h"
#include <math.h>
#include <algorithm>

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

std::vector<double> MathFunctions::get_empirical_distribution_function(const std::vector<double>& data, const std::vector<double>& intervals)
{
	if (intervals.size() < 2)
		throw "Нет границ интервалов разбиения";

	std::vector<double> sorted_data{ data };
	std::sort(sorted_data.begin(), sorted_data.end());

	const size_t ELEMENTS_COUNT{ sorted_data.size() };
	std::vector<double> empirical_distribution_function(intervals.size() - 1);
	for (size_t i{ 1 }, data_counter{ 0 };
		i < intervals.size() && data_counter < ELEMENTS_COUNT;
		++i)
	{
		const double right_border{ intervals[i] };
		while (data_counter < ELEMENTS_COUNT && sorted_data[data_counter] < right_border)
			++data_counter;
		empirical_distribution_function[i - 1] = (data_counter / (double)ELEMENTS_COUNT);
	}
	return empirical_distribution_function;
}

double MathFunctions::uniform_distribution_function(double x, double a, double b)
{
	double result;
	if (x >= a && x <= b)
		result = (x - a) / (b - a);
	else
		result = x < a ? 0 : 1;
	return result;
}

double MathFunctions::pearson_criteria(const std::vector<double>& data, const std::vector<double>& intervals, const std::function<double(double)>& theoretical_distribution_function)
{
	std::vector<double> empirical_distribution_function{ MathFunctions::get_empirical_distribution_function(data, intervals) };

	// расчёт теоретической функции распределения
	std::vector<double> calculated_theoretical_distribution_function(empirical_distribution_function.size());
	for (size_t i{ 1 }; i < intervals.size(); ++i)
		calculated_theoretical_distribution_function[i - 1] = theoretical_distribution_function(intervals[i]);

	double result{ 0 };
	for (size_t i{ 0 }; i < calculated_theoretical_distribution_function.size(); ++i)
	{
		double delta{ calculated_theoretical_distribution_function[i] - empirical_distribution_function[i] };
		result += delta * delta / calculated_theoretical_distribution_function[i];
	}
	return result * data.size();
}
