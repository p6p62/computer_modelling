#include <iostream>
#include "CoveyouGenerator.h"
#include "MathFunctions.h"
#include <algorithm>
#include <matplot/matplot.h>

int main()
{
	constexpr int ELEMENTS_COUNT{ 5000 };
	constexpr int HISTOGRAM_PARTS_COUNT{ 12 };

	constexpr long SEED{ 10 };
	constexpr long DEGREE{ 12 };

	setlocale(LC_ALL, "Russian");
	setlocale(LC_NUMERIC, "English");

	// генерация случайных чисел
	CoveyouGenerator generator{ SEED, DEGREE };
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
