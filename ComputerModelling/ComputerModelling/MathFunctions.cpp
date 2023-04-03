#include "MathFunctions.h"
#include <math.h>
#include <algorithm>
#include <map>
#include <set>
#include <numbers>
#include <boost/math/special_functions/factorials.hpp>

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

double MathFunctions::combination(unsigned long m, unsigned long n)
{
	return boost::math::factorial<double>(n) / (boost::math::factorial<double>(m) * boost::math::factorial<double>(n - m));
}

std::vector<double> MathFunctions::get_empirical_distribution_function(const std::vector<double>& data, const std::vector<double>& intervals)
{
	if (intervals.size() < 2)
		throw "Ќет границ интервалов разбиени€";

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

void MathFunctions::get_statistical_distribution_function_points(const std::vector<double>& data, std::map<double, double>& out_values)
{
	std::vector<double> sorted_data(data);
	std::sort(sorted_data.begin(), sorted_data.end());

	out_values.clear();
	out_values.emplace(data[0], 0);

	double* function_value{ &out_values[data[0]] };
	for (size_t i{ 0 }, old_step_index{ 0 }; i < sorted_data.size(); ++i)
	{
		++(*function_value);
		if (sorted_data[old_step_index] < sorted_data[i])
		{
			double temp{ *function_value };
			function_value = &(out_values.emplace(sorted_data[i], 0).first->second);
			*function_value = temp;
			old_step_index = i;
		}
	}
	for (auto& v : out_values)
		v.second /= data.size();
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
	std::vector<double> real_normalized_histogram;
	MathFunctions::get_histogram_on_custom_intervals(data, intervals, real_normalized_histogram);
	const size_t DATA_SIZE{ data.size() };
	for (double& i : real_normalized_histogram)
		i /= DATA_SIZE;

	// расчЄт теоретической гистограммы
	std::vector<double> theoretical_histogram(intervals.size() - 1);
	for (size_t i{ 1 }; i < intervals.size(); ++i)
		theoretical_histogram[i - 1] = theoretical_distribution_function(intervals[i]) - theoretical_distribution_function(intervals[i - 1]);

	double result{ 0 };
	for (size_t i{ 0 }; i < real_normalized_histogram.size(); ++i)
	{
		double delta{ theoretical_histogram[i] - real_normalized_histogram[i] };
		result += delta * delta / real_normalized_histogram[i];
	}
	return result * data.size();
}

double MathFunctions::colmogorov_criteria(const std::vector<double>& data, const std::function<double(double)>& theoretical_distribution_function)
{
	// точки статистической функции распределени€ в виде [аргумент-значение]
	std::map<double, double> statistical_distribution_function;
	MathFunctions::get_statistical_distribution_function_points(data, statistical_distribution_function);

	// поиск максимального отклонени€
	double max_difference{ 0 };
	for (auto i1{ statistical_distribution_function.begin() }, i2{ std::next(i1) }; i2 != statistical_distribution_function.end(); ++i2)
	{
		double temp{ abs(i1->second - theoretical_distribution_function(i2->first)) };
		if (temp > max_difference)
			max_difference = temp;
		temp = abs(i2->second - theoretical_distribution_function(i2->first));
		if (temp > max_difference)
			max_difference = temp;
		i1 = i2;
	}

	return max_difference * sqrt(data.size());
}

static double collector_probability_density_function(double length, unsigned int digits_count)
{
	double sum{ 0 };
	for (size_t i = 0; i < digits_count - 1; i++)
	{
		sum += pow(-1, i) * MathFunctions::combination(i, digits_count - 1) * pow(digits_count - 1 - i, length - 1);
	}
	return sum / pow(digits_count, length - 1);
}

static double collector_distribution_function(double length, unsigned int digits_count)
{
	double result{ 0 };
	for (size_t i{ digits_count }; i <= static_cast<size_t>(length); ++i)
		result += collector_probability_density_function(i, digits_count);
	return result;
}

double MathFunctions::collector_criteria(const std::vector<double>& data, unsigned int digit_count)
{
	const size_t DATA_SIZE{ data.size() };

	// получение набора данных с длинами последовательностей из выборки
	std::vector<unsigned long long> length_sequence_values;
	std::set<unsigned int> digits;
	size_t i{ 0 };
	while (i < DATA_SIZE)
	{
		unsigned long long sequence_length{ 0 };
		while (i < DATA_SIZE && digits.size() < digit_count)
		{
			digits.insert(data[i++] * digit_count);
			++sequence_length;
		}
		if (digits.size() >= digit_count)
			length_sequence_values.push_back(sequence_length);
		digits.clear();
	}

	// подготовка интервалов и проверка длин по критерию ’и-квадрат
	std::vector<double> lengths{ length_sequence_values.begin(), length_sequence_values.end() };
	std::set<unsigned int> different_lengths{ length_sequence_values.begin(), length_sequence_values.end() };
	std::vector<double> intervals{ different_lengths.begin(), different_lengths.end() };
	std::sort(intervals.begin(), intervals.end());

	double pearson_value{ MathFunctions::pearson_criteria(lengths, intervals,
		[digit_count](double r) -> double { return collector_distribution_function(r, digit_count); }) };

	return pearson_value;
}

double MathFunctions::normal_probability_density_function(double x, double math_expectation, double variance)
{
	return exp(-pow((x - math_expectation), 2) / (2 * variance)) / (sqrt(2 * std::numbers::pi * variance));
}

double MathFunctions::normal_distribution_function(double x, double math_expectation, double variance)
{
	return 0.5 * (1 + std::erf((x - math_expectation) / sqrt(2 * variance)));
}

double MathFunctions::gamma_probability_density_function(double x, double k_parameter, double theta_parameter)
{
	double result{ 0 };
	if (x > 0)
		result = pow(x, k_parameter - 1) * exp(-x / theta_parameter) / (pow(theta_parameter, k_parameter) * tgamma(k_parameter));
	return result;
}

double MathFunctions::exponential_probability_density_function(double x, double lambda_parameter)
{
	return lambda_parameter * exp(-lambda_parameter * x);
}

double MathFunctions::exponential_probability_distribution_function(double x, double lambda_parameter)
{
	return 1 - exp(-lambda_parameter * x);
}
