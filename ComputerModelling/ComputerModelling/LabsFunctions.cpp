#include "LabsFunctions.h"
#include <iostream>
#include "CoveyouGenerator.h"
#include "MathFunctions.h"
#include <algorithm>
#include <matplot/matplot.h>

void lab1_program()
{
	constexpr int ELEMENTS_COUNT{ 5000 };
	constexpr int HISTOGRAM_PARTS_COUNT{ 12 };

	// генерация случайных чисел
	CoveyouGenerator generator;
	std::vector<double> random_numbers(ELEMENTS_COUNT);
	for (size_t i = 0; i < ELEMENTS_COUNT; i++)
		random_numbers[i] = generator.next();

	// расчёт данных статистической функции распределения
	std::vector<int> histogram;
	MathFunctions::get_histogram(random_numbers, HISTOGRAM_PARTS_COUNT, histogram);
	std::vector<double> emripical_distribution_function;
	for (size_t i{ 0 }, counter{ 0 }; i < histogram.size(); ++i)
	{
		counter += histogram[i];
		emripical_distribution_function.push_back(counter / (double)ELEMENTS_COUNT);
	}

	// вывод статистических характеристик
	const double math_expect{ MathFunctions::math_expectation(random_numbers) };
	std::cout << "Математическое ожидание: " << math_expect << std::endl;
	std::cout << "Дисперсия: " << MathFunctions::variance(random_numbers, &math_expect) << std::endl;
	std::cout << "Второй начальный момент: " << MathFunctions::moment_initial(random_numbers, 2) << std::endl;
	std::cout << "Третий начальный момент: " << MathFunctions::moment_initial(random_numbers, 3) << std::endl;

	// подготовка графика
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

	// оценка по критерию Пирсона
	const std::vector<double> intervals{ MathFunctions::separate_on_parts(INTERVALS_COUNT) };
	const double pearson_value{ MathFunctions::pearson_criteria(random_data, intervals, uniform_distribution_function) };
	std::cout << "Значение критерия Пирсона: " << pearson_value << std::endl;
	std::cout << "Количество степеней свободы: " << INTERVALS_COUNT - 1 << std::endl;

	// оценка по критерию Колмогорова
	const double colmogorov_value{ MathFunctions::colmogorov_criteria(random_data, uniform_distribution_function) };
	std::cout << "Значение критерия Колмогорова: " << colmogorov_value << std::endl;

	// оценка по критерию коллекционера
	constexpr int COLLECTED_DIGITS_COUNT{ 24 };
	const double collector_value{ MathFunctions::collector_criteria(random_data, COLLECTED_DIGITS_COUNT) };
	std::cout << "Значение критерия коллекционера: " << collector_value << std::endl;

	// график
	/*auto histogram_graph{ matplot::hist(random_data, INTERVALS_COUNT) };
	histogram_graph->bin_edges(intervals);
	matplot::show();*/
}