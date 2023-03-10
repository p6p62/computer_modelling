#include "LabsFunctions.h"
#include <iostream>
#include "CoveyouGenerator.h"
#include "MathFunctions.h"
#include <algorithm>
#include <matplot/matplot.h>
#include <chrono>

void lab1_program()
{
	constexpr int ELEMENTS_COUNT{ 5000 };
	constexpr int HISTOGRAM_PARTS_COUNT{ 12 };

	// ????????? ????????? ?????
	CoveyouGenerator generator;
	std::vector<double> random_numbers(ELEMENTS_COUNT);
	for (size_t i = 0; i < ELEMENTS_COUNT; i++)
		random_numbers[i] = generator.next();

	// ?????? ?????? ?????????????? ??????? ?????????????
	std::vector<int> histogram;
	MathFunctions::get_histogram(random_numbers, HISTOGRAM_PARTS_COUNT, histogram);
	std::vector<double> emripical_distribution_function;
	for (size_t i{ 0 }, counter{ 0 }; i < histogram.size(); ++i)
	{
		counter += histogram[i];
		emripical_distribution_function.push_back(counter / (double)ELEMENTS_COUNT);
	}

	// ????? ?????????????? ?????????????
	const double math_expect{ MathFunctions::math_expectation(random_numbers) };
	std::cout << "?????????????? ????????: " << math_expect << std::endl;
	std::cout << "?????????: " << MathFunctions::variance(random_numbers, &math_expect) << std::endl;
	std::cout << "?????? ????????? ??????: " << MathFunctions::moment_initial(random_numbers, 2) << std::endl;
	std::cout << "?????? ????????? ??????: " << MathFunctions::moment_initial(random_numbers, 3) << std::endl;

	// ?????????? ???????
	auto histogram_graph{ matplot::hist(random_numbers, HISTOGRAM_PARTS_COUNT) };
	histogram_graph->bin_edges(MathFunctions::separate_on_parts(HISTOGRAM_PARTS_COUNT));
	histogram_graph->normalization(matplot::histogram::normalization::pdf);
	matplot::hold(true);
	auto distribution_function_graph{ matplot::fplot(
		[&emripical_distribution_function](double x) -> double
		{
			size_t index{(size_t)(x * HISTOGRAM_PARTS_COUNT)};
			if (index < 0)
				index = 0;
			else if (index > HISTOGRAM_PARTS_COUNT - 1)
				index = HISTOGRAM_PARTS_COUNT - 1;
			return emripical_distribution_function[index];
		}, std::array<double, 2>{0, 1}) };
	distribution_function_graph->line_width(3);
	matplot::show();
}

void lab2_program()
{
	constexpr long RANDOM_DATA_SIZE{ 8000 };
	const int INTERVALS_COUNT{ static_cast<int>(1 + log2(RANDOM_DATA_SIZE)) };

	CoveyouGenerator generator;
	std::vector<double> random_data(RANDOM_DATA_SIZE);
	for (size_t i{ 0 }; i < RANDOM_DATA_SIZE; ++i)
		random_data[i] = generator.next();

	const std::function<double(double)>& uniform_distribution_function{ [](double x) -> double { return MathFunctions::uniform_distribution_function(x); } };

	// ?????? ?? ???????? ???????
	const std::vector<double> intervals{ MathFunctions::separate_on_parts(INTERVALS_COUNT) };
	const double pearson_value{ MathFunctions::pearson_criteria(random_data, intervals, uniform_distribution_function) };
	std::cout << "???????? ???????? ???????: " << pearson_value << std::endl;
	std::cout << "?????????? ???????? ???????: " << INTERVALS_COUNT - 1 << std::endl;

	// ?????? ?? ???????? ???????????
	const double colmogorov_value{ MathFunctions::colmogorov_criteria(random_data, uniform_distribution_function) };
	std::cout << "???????? ???????? ???????????: " << colmogorov_value << std::endl;

	// ?????? ?? ???????? ?????????????
	constexpr int COLLECTED_DIGITS_COUNT{ 24 };
	const double collector_value{ MathFunctions::collector_criteria(random_data, COLLECTED_DIGITS_COUNT) };
	std::cout << "???????? ???????? ?????????????: " << collector_value << std::endl;

	// ??????
	/*auto histogram_graph{ matplot::hist(random_data, INTERVALS_COUNT) };
	histogram_graph->bin_edges(intervals);
	matplot::show();*/
}

static double lab3_variant6_distribution_function(double x)
{
	double result{ 0 };
	if (x >= 0)
	{
		if (x < 0.3)
			result = 0.2 * pow(10, x) - 0.2;
		else if (x < 0.7)
			result = 1.5 * x - 0.25;
		else if (x < 1.5)
			result = 0.25 * x + 0.625;
		else
			result = 1;
	}
	return result;
}

static double lab3_variant6_inverse_distribution_function(double x)
{
	double result{ 0 };
	if (x >= lab3_variant6_distribution_function(0))
	{
		if (x < lab3_variant6_distribution_function(0.3))
			result = log10(x * 5 + 1);
		else if (x < lab3_variant6_distribution_function(0.7))
			result = (x + 0.25) / 1.5;
		else if (x <= lab3_variant6_distribution_function(1.5))
			result = 4 * (x - 0.625);
		else
			result = 1.5;
	}
	return result;
}

void lab3_program()
{
	constexpr int ELEMENTS_COUNT{ 50000 };
	constexpr int INTERVALS_COUNT{ 25 };
	constexpr double LEFT_BORDER{ 0 };
	constexpr double RIGHT_BORDER{ 1.5 };

	// ????????? ????????? ??????
	CoveyouGenerator generator;
	std::vector<double> random_data(ELEMENTS_COUNT);
	for (double& v : random_data)
		v = lab3_variant6_inverse_distribution_function(generator.next());

	// ????? ?????????????? ?????????????
	const double math_expect{ MathFunctions::math_expectation(random_data) };
	std::cout << "?????????????? ????????: " << math_expect << std::endl;
	std::cout << "?????????: " << MathFunctions::variance(random_data, &math_expect) << std::endl;

	// ???????? ?? ???????? ???????
	const double pearson_value{ MathFunctions::pearson_criteria(random_data, MathFunctions::separate_on_parts(INTERVALS_COUNT, LEFT_BORDER, RIGHT_BORDER), lab3_variant6_distribution_function) };
	std::cout << "???????? ???????? ???????: " << pearson_value << std::endl;

	// ??????
	auto histogram_graph{ matplot::hist(random_data, INTERVALS_COUNT) };
	histogram_graph->bin_edges(MathFunctions::separate_on_parts(INTERVALS_COUNT, LEFT_BORDER, RIGHT_BORDER));
	histogram_graph->normalization(matplot::histogram::normalization::probability);
	matplot::hold(true);

	std::vector<double> histogram_borders{ MathFunctions::separate_on_parts(INTERVALS_COUNT, LEFT_BORDER, RIGHT_BORDER) };
	auto histogram_graph2{ matplot::fplot([&histogram_borders](double x) -> double
		{
			return lab3_variant6_distribution_function(x);
		}, std::array<double, 2>{LEFT_BORDER, RIGHT_BORDER}) };

	std::vector<double> emripical_distribution_function{ MathFunctions::get_empirical_distribution_function(random_data, histogram_borders) };
	auto distribution_function_graph{ matplot::fplot(
		[&emripical_distribution_function](double x) -> double
		{
			size_t index{(size_t)(x * INTERVALS_COUNT / (RIGHT_BORDER - LEFT_BORDER))};
			if (index < 0)
				index = 0;
			else if (index > INTERVALS_COUNT - 1)
				index = INTERVALS_COUNT - 1;
			return emripical_distribution_function[index];
		}, std::array<double, 2>{LEFT_BORDER, RIGHT_BORDER}) };
	histogram_graph->bin_edges(MathFunctions::separate_on_parts(INTERVALS_COUNT, LEFT_BORDER, RIGHT_BORDER));
	matplot::show();
}
