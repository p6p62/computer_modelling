#include "LabsFunctions.h"
#include <iostream>
#include "CoveyouGenerator.h"
#include "MarsagliaBrayGenerator.h"
#include "BetaDistributionGenerator.h"
#include "MathFunctions.h"
#include "ShootingCompetitions.h"
#include "RaindropFall.h"
#include "QueueingSystem.h"
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

	// получение случайных данных
	CoveyouGenerator generator;
	std::vector<double> random_data(ELEMENTS_COUNT);
	for (double& v : random_data)
		v = lab3_variant6_inverse_distribution_function(generator.next());

	// вывод статистических характеристик
	const double math_expect{ MathFunctions::math_expectation(random_data) };
	std::cout << "Математическое ожидание: " << math_expect << std::endl;
	std::cout << "Дисперсия: " << MathFunctions::variance(random_data, &math_expect) << std::endl;

	// проверка по критерию Пирсона
	const double pearson_value{ MathFunctions::pearson_criteria(random_data, MathFunctions::separate_on_parts(INTERVALS_COUNT, LEFT_BORDER, RIGHT_BORDER), lab3_variant6_distribution_function) };
	std::cout << "Значение критерия Пирсона: " << pearson_value << std::endl;

	// график
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

void lab4_program()
{
	constexpr int ELEMENTS_COUNT{ 5000 };
	constexpr int HISTOGRAM_PARTS_COUNT{ 20 };

	constexpr double MATH_EXPECTATION{ 2 };
	constexpr double VARIANCE{ 0.9 };

	// генерация случайных чисел
	CoveyouGenerator uniform_generator;
	MarsagliaBrayGenerator generator{ MATH_EXPECTATION, VARIANCE, [&uniform_generator]() -> double { return uniform_generator.next(); } };
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

	constexpr double LEFT_GRAPH_BORDER{ -2 };
	constexpr double RIGHT_GRAPH_BORDER{ 6 };

	// оценка по критерию Колмогорова
	const double colmogorov_value{ MathFunctions::colmogorov_criteria(random_numbers,
		[](double x) -> double { return MathFunctions::normal_distribution_function(x, MATH_EXPECTATION, VARIANCE); }) };
	std::cout << "Значение критерия Колмогорова: " << colmogorov_value << std::endl;

	// подготовка графика
	auto histogram_graph{ matplot::hist(random_numbers, HISTOGRAM_PARTS_COUNT) };
	histogram_graph->bin_edges(MathFunctions::separate_on_parts(HISTOGRAM_PARTS_COUNT, LEFT_GRAPH_BORDER, RIGHT_GRAPH_BORDER));
	histogram_graph->normalization(matplot::histogram::normalization::pdf);
	matplot::hold(true);
	auto distribution_function_graph{ matplot::fplot(
		[&emripical_distribution_function](double x) -> double
		{
			size_t index{(size_t)((x - LEFT_GRAPH_BORDER) * HISTOGRAM_PARTS_COUNT / (RIGHT_GRAPH_BORDER - LEFT_GRAPH_BORDER))};
			if (index < 0)
				index = 0;
			else if (index > HISTOGRAM_PARTS_COUNT - 1)
				index = HISTOGRAM_PARTS_COUNT - 1;
			return emripical_distribution_function[index];
		}, std::array<double, 2>{LEFT_GRAPH_BORDER, RIGHT_GRAPH_BORDER}) };
	distribution_function_graph->line_width(3);
	matplot::show();
}

void lab5_beta_distribution(int elements_count, int histogram_parts_count)
{
	const int ELEMENTS_COUNT{ elements_count };
	const int HISTOGRAM_PARTS_COUNT{ histogram_parts_count };

	constexpr double P_PARAMETER{ 0.5 };
	constexpr double M_PARAMETER{ 0.5 };

	// генерация случайных чисел
	CoveyouGenerator uniform_generator;
	BetaDistributionGenerator generator{ P_PARAMETER, M_PARAMETER, [&uniform_generator]() -> double { return uniform_generator.next(); } };
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

	constexpr double LEFT_GRAPH_BORDER{ 0 };
	constexpr double RIGHT_GRAPH_BORDER{ 1 };

	// оценка по критерию Колмогорова
	const double colmogorov_value{ MathFunctions::colmogorov_criteria(random_numbers,
		[](double x) -> double
		{
			constexpr double DELTA{1e-2};
			double result{ 0 };
			for (double i{ DELTA }; i <= x; i += DELTA)
				result += pow(i, P_PARAMETER - 1) * pow(1 - i, M_PARAMETER - 1);
			return result * DELTA / std::beta(P_PARAMETER, M_PARAMETER);
		}) };
	std::cout << "Значение критерия Колмогорова: " << colmogorov_value << std::endl;

	// подготовка графика
	auto histogram_graph{ matplot::hist(random_numbers, HISTOGRAM_PARTS_COUNT) };
	histogram_graph->bin_edges(MathFunctions::separate_on_parts(HISTOGRAM_PARTS_COUNT, LEFT_GRAPH_BORDER, RIGHT_GRAPH_BORDER));
	histogram_graph->normalization(matplot::histogram::normalization::pdf);
	matplot::hold(true);
	auto distribution_function_graph{ matplot::fplot(
		[HISTOGRAM_PARTS_COUNT, &emripical_distribution_function](double x) -> double
		{
			size_t index{(size_t)((x - LEFT_GRAPH_BORDER) * HISTOGRAM_PARTS_COUNT / (RIGHT_GRAPH_BORDER - LEFT_GRAPH_BORDER))};
			if (index < 0)
				index = 0;
			else if (index > HISTOGRAM_PARTS_COUNT - 1)
				index = HISTOGRAM_PARTS_COUNT - 1;
			return emripical_distribution_function[index];
		}, std::array<double, 2>{LEFT_GRAPH_BORDER, RIGHT_GRAPH_BORDER}) };
	distribution_function_graph->line_width(3);
	matplot::show();
}

void lab5_logarithmically_normal_distribution(int elements_count, int histogram_parts_count)
{
	const int ELEMENTS_COUNT{ elements_count };
	const int HISTOGRAM_PARTS_COUNT{ histogram_parts_count };

	constexpr double MATH_EXPECTATION{ 2 };
	constexpr double VARIANCE{ 0.9 };

	// генерация случайных чисел
	CoveyouGenerator uniform_generator;
	MarsagliaBrayGenerator generator{ MATH_EXPECTATION, VARIANCE, [&uniform_generator]() -> double { return uniform_generator.next(); } };
	std::vector<double> random_numbers(ELEMENTS_COUNT);
	for (size_t i = 0; i < ELEMENTS_COUNT; i++)
		random_numbers[i] = exp(generator.next());

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

	constexpr double LEFT_GRAPH_BORDER{ -2 };
	constexpr double RIGHT_GRAPH_BORDER{ 15 };

	// оценка по критерию Колмогорова
	// !!!ДОПИСАТЬ ФУНКЦИЮ РАСПРЕДЕЛЕНИЯ ЛОГАРИФМИЧЕСКИ НОРМАЛЬНОГО ЗАКОНА!!!
	const double colmogorov_value{ MathFunctions::colmogorov_criteria(random_numbers,
		[](double x) -> double { return MathFunctions::normal_distribution_function(x, MATH_EXPECTATION, VARIANCE); }) };
	std::cout << "Значение критерия Колмогорова: " << colmogorov_value << std::endl;

	// подготовка графика
	auto histogram_graph{ matplot::hist(random_numbers, HISTOGRAM_PARTS_COUNT) };
	histogram_graph->bin_edges(MathFunctions::separate_on_parts(HISTOGRAM_PARTS_COUNT, LEFT_GRAPH_BORDER, RIGHT_GRAPH_BORDER));
	histogram_graph->normalization(matplot::histogram::normalization::pdf);
	matplot::hold(true);
	auto distribution_function_graph{ matplot::fplot(
		[HISTOGRAM_PARTS_COUNT, &emripical_distribution_function](double x) -> double
		{
			size_t index{(size_t)((x - LEFT_GRAPH_BORDER) * HISTOGRAM_PARTS_COUNT / (RIGHT_GRAPH_BORDER - LEFT_GRAPH_BORDER))};
			if (index < 0)
				index = 0;
			else if (index > HISTOGRAM_PARTS_COUNT - 1)
				index = HISTOGRAM_PARTS_COUNT - 1;
			return emripical_distribution_function[index];
		}, std::array<double, 2>{LEFT_GRAPH_BORDER, RIGHT_GRAPH_BORDER}) };
	distribution_function_graph->line_width(3);
	matplot::show();
}

void lab5_program()
{
	constexpr int ELEMENTS_COUNT{ 5000 };
	constexpr int HISTOGRAM_PARTS_COUNT{ 15 };

	//lab5_beta_distribution(ELEMENTS_COUNT, HISTOGRAM_PARTS_COUNT);
	lab5_logarithmically_normal_distribution(ELEMENTS_COUNT, HISTOGRAM_PARTS_COUNT);
}

void lab6_program()
{
	constexpr int SELECTION_SIZE{ 20000 };
	constexpr int TESTS_COUNT_IN_COMPETITION{ 300 };
	constexpr int BULLETS_COUNT{ 5 };
	//const std::vector<double> HIT_PROBABILITIES{ 0.6, 0.55, 0.62, 0.65, 0.58, 0.59 };
	const std::vector<double> HIT_PROBABILITIES{ 0.16, 0.05, 0.12, 0.85, 0.08, 0.09 };
	constexpr int TRACKED_SHOOTER{ 3 };

	// набор вероятностей выигрыша
	int rand_seed{ 120 }; // для получения повторимых, но различных данных с последовательности опытов
	std::vector<double> winning_probability;
	for (size_t i{ 0 }; i < SELECTION_SIZE; ++i, ++rand_seed)
	{
		winning_probability.push_back(ShootingCompetitions::test_shooting(TESTS_COUNT_IN_COMPETITION, BULLETS_COUNT, HIT_PROBABILITIES, TRACKED_SHOOTER, rand_seed));
	}

	// расчёт доверительного интервала и статистических характеристик
	constexpr double quantile_infinity_0_95{ 1.645 };
	const double math_expectation{ MathFunctions::math_expectation(winning_probability) };
	const double fixed_variance{ (double)SELECTION_SIZE / (SELECTION_SIZE - 1) * MathFunctions::variance(winning_probability, &math_expectation) };
	const double mean_square_deviation{ sqrt(fixed_variance) };
	const double delta{ mean_square_deviation * quantile_infinity_0_95 / sqrt(SELECTION_SIZE) };
	std::cout << "Математическое ожидание: " << math_expectation << std::endl;
	std::cout << "Исправленная выборочная дисперсия: " << fixed_variance << std::endl;
	std::cout << std::format("Доверительный интервал с 0.95 надёжностью: [{:.6f}, {:.6f}]\n", math_expectation - delta, math_expectation + delta);

	// проверка аналитическим решением
	std::cout << "Вероятность выигрыша, рассчитанная аналитически: " << ShootingCompetitions::test_shooting_by_mathematical(BULLETS_COUNT, HIT_PROBABILITIES, TRACKED_SHOOTER) << std::endl;

	// график
	constexpr int HISTOGRAM_PART_COUNT{ 25 };
	auto minmax_pair{ std::minmax_element(winning_probability.begin(), winning_probability.end()) };
	std::vector<double> borders{ MathFunctions::separate_on_parts(HISTOGRAM_PART_COUNT, *minmax_pair.first, *minmax_pair.second) };
	auto histogram_graph{ matplot::hist(winning_probability, HISTOGRAM_PART_COUNT) };
	histogram_graph->bin_edges(borders);
	matplot::show();
}

void lab7_program()
{
	constexpr int DROP_TEST_COUNT{ 8000 };
	constexpr int INITIAL_HEIGHT{ 15 };
	constexpr double P_DOWN{ 0.5 };
	constexpr double P_UP{ 0.1 };
	constexpr double P_LEFT{ 0.2 };
	constexpr double P_RIGHT{ 0.2 };

	// моделирование падений
	std::vector<long> falling_times;
	RaindropFall::test_falling(DROP_TEST_COUNT, INITIAL_HEIGHT, P_DOWN, P_UP, P_LEFT, P_RIGHT, falling_times);

	// вывод статистических характеристик
	std::vector<double> falling_times_in_double{ falling_times.begin(), falling_times.end() };
	const double math_expect{ MathFunctions::math_expectation(falling_times_in_double) };
	const double variance{ MathFunctions::variance(falling_times_in_double, &math_expect) };
	std::cout << "Математическое ожидание: " << math_expect << std::endl;
	std::cout << "Дисперсия: " << variance << std::endl;

	// определение функциональной зависимости времени от высоты
	std::map<int, long> approximate_height_to_time_function;
	constexpr int HEIGHT_MIN{ 4 }, HEIGHT_MAX{ 35 };
	std::vector<long> times_for_function;
	std::vector<double> temp(DROP_TEST_COUNT);
	for (int h{ HEIGHT_MIN }; h <= HEIGHT_MAX; ++h)
	{
		RaindropFall::test_falling(DROP_TEST_COUNT, h, P_DOWN, P_UP, P_LEFT, P_RIGHT, times_for_function);
		temp.assign(times_for_function.begin(), times_for_function.end());
		double mean_fall_time{ MathFunctions::math_expectation(temp) };
		approximate_height_to_time_function.insert({ h, mean_fall_time });
	}

	// подсчёт параметров гамма-распределения
	const double THETA{ variance / math_expect };
	const double K{ math_expect / THETA };
	std::cout << "Параметры аппроксимирующего гамма-распределения: " << std::endl;
	std::cout << "k: " << K << std::endl;
	std::cout << "theta: " << THETA << std::endl;

	// интервалы для гистограммы
	auto minmax_pair{ std::minmax_element(falling_times.begin(), falling_times.end()) };
	double min{ (double)*minmax_pair.first };
	double max{ (double)*minmax_pair.second };
	const int HISTOGRAM_PART_COUNT{ (int)(max - min) / 2 };
	std::vector<double> borders{ MathFunctions::separate_on_parts(HISTOGRAM_PART_COUNT, min, max) };

	std::vector<double> emripical_distribution_function{ MathFunctions::get_empirical_distribution_function(falling_times_in_double, borders) };

	// график
	auto histogram_graph{ matplot::hist(falling_times) };
	histogram_graph->bin_edges(borders);
	histogram_graph->normalization(matplot::histogram::normalization::pdf);
	matplot::hold(true);

	auto gamma_theoretical_graph{ matplot::fplot(
		[K, THETA](double x) -> double {return MathFunctions::gamma_probability_density_function(x, K, THETA); },
		std::array<double, 2>{min, max}) };
	gamma_theoretical_graph->line_width(3);

	matplot::figure();
	auto distribution_function_graph{ matplot::fplot(
		[min, max, HISTOGRAM_PART_COUNT, &emripical_distribution_function](double x) -> double
		{
			size_t index{(size_t)((x - min) * HISTOGRAM_PART_COUNT / (max - min))};
			if (index < 0)
				index = 0;
			else if (index > HISTOGRAM_PART_COUNT - 1)
				index = HISTOGRAM_PART_COUNT - 1;
			return emripical_distribution_function[index];
		}, std::array<double, 2>{min, max}) };
	distribution_function_graph->line_width(3);

	matplot::figure();
	std::vector<int> x, y;
	for (const auto& it : approximate_height_to_time_function)
	{
		x.push_back(it.first);
		y.push_back(it.second);
	}
	std::vector<size_t> indices(x.size());
	for (size_t i{ 0 }; i < indices.size(); ++i)
		indices[i] = i;
	auto height_to_time_function_graph{ matplot::plot(x, y, "-o") };
	height_to_time_function_graph->marker_indices(indices);
	height_to_time_function_graph->marker_size(7);
	height_to_time_function_graph->line_width(3);
	matplot::show();
}

void lab8_math_expectation()
{
	constexpr double ACCURACY{ 5e-2 };
	constexpr double QUANTILE_INFINITY_0_95{ 1.645 }; // достоверность задаётся значением квантиля
	constexpr int SELECTION_FIRST_TEST_SIZE{ 50 };

	constexpr int DROP_TEST_COUNT{ 20 };
	constexpr int INITIAL_HEIGHT{ 15 };
	constexpr double P_DOWN{ 0.5 };
	constexpr double P_UP{ 0.1 };
	constexpr double P_LEFT{ 0.2 };
	constexpr double P_RIGHT{ 0.2 };

	// выполнение пробного эксперимента для оценки дисперсии
	std::vector<long> falling_times(DROP_TEST_COUNT);
	auto start_it{ falling_times.begin() };
	auto end_it{ falling_times.end() };
	std::vector<double> falling_times_math_expectations(SELECTION_FIRST_TEST_SIZE);
	for (double& m : falling_times_math_expectations)
	{
		RaindropFall::test_falling(DROP_TEST_COUNT, INITIAL_HEIGHT, P_DOWN, P_UP, P_LEFT, P_RIGHT, falling_times);
		m = std::accumulate(start_it, end_it, 0) / falling_times.size();
	}

	// определение минимально необходимого размера выборки и проведение дополнительных итераций по необходимости
	const double ESTIMATED_VARIANCE{ MathFunctions::variance(falling_times_math_expectations) };
	const size_t MIN_SELECTION_SIZE{ static_cast<size_t>(pow(QUANTILE_INFINITY_0_95, 2) * ESTIMATED_VARIANCE / pow(ACCURACY, 2)) };
	if (MIN_SELECTION_SIZE > SELECTION_FIRST_TEST_SIZE)
	{
		falling_times_math_expectations.resize(MIN_SELECTION_SIZE);
		for (size_t i{ SELECTION_FIRST_TEST_SIZE }; i < falling_times_math_expectations.size(); ++i)
		{
			RaindropFall::test_falling(DROP_TEST_COUNT, INITIAL_HEIGHT, P_DOWN, P_UP, P_LEFT, P_RIGHT, falling_times);
			falling_times_math_expectations[i] = std::accumulate(start_it, end_it, 0) / falling_times.size();
		}
	}

	const double MATH_EXPECTATION{ MathFunctions::math_expectation(falling_times_math_expectations) };
	const double VARIANCE{ MathFunctions::variance(falling_times_math_expectations, &MATH_EXPECTATION) * SELECTION_FIRST_TEST_SIZE / (SELECTION_FIRST_TEST_SIZE - 1) };

	std::cout << "Заданный уровень надёжности: 0.95\n";
	std::cout << std::format("Заданная точность: {}\n", ACCURACY);
	std::cout << std::format("Размер выборки в пробном эксперименте: {}\n", SELECTION_FIRST_TEST_SIZE);
	std::cout << std::format("Оценка дисперсии: {}\n", ESTIMATED_VARIANCE);
	std::cout << std::format("Минимально необходимый размер выборки: {}\n\n", MIN_SELECTION_SIZE);

	std::cout << std::format("Математическое ожидание выборочных средних после выполнения эксперимента:\n{}\n", MATH_EXPECTATION);
	std::cout << std::format("Дисперсия выборочных средних после выполнения эксперимента:\n{}\n", VARIANCE);
	std::cout << std::format("Доверительный интервал:\n[{}, {}]\n", MATH_EXPECTATION - ACCURACY, MATH_EXPECTATION + ACCURACY);

	// график
	auto min_max{ std::minmax_element(falling_times_math_expectations.begin(), falling_times_math_expectations.end()) };
	auto histogram_graph{ matplot::hist(falling_times_math_expectations) };
	histogram_graph->normalization(matplot::histogram::normalization::pdf);
	matplot::hold(true);
	auto approximate_normal_distribution_graph{ matplot::fplot(
		[MATH_EXPECTATION, VARIANCE](double x) -> double
		{
			return MathFunctions::normal_probability_density_function(x, MATH_EXPECTATION, VARIANCE);
		}, std::array<double, 2>{*min_max.first,* min_max.second}) };
	approximate_normal_distribution_graph->line_width(3);
}

void lab8_variance()
{
	constexpr double ACCURACY{ 2 };
	constexpr double QUANTILE_INFINITY_0_95{ 1.645 }; // достоверность задаётся значением квантиля
	constexpr int SELECTION_FIRST_TEST_SIZE{ 50 };

	constexpr int DROP_TEST_COUNT{ 20 };
	constexpr int INITIAL_HEIGHT{ 15 };
	constexpr double P_DOWN{ 0.5 };
	constexpr double P_UP{ 0.1 };
	constexpr double P_LEFT{ 0.2 };
	constexpr double P_RIGHT{ 0.2 };

	// выполнение пробного эксперимента для оценки дисперсии
	std::vector<long> falling_times(DROP_TEST_COUNT);
	std::vector<double> falling_times_double(DROP_TEST_COUNT);
	auto start_it{ falling_times.begin() };
	auto end_it{ falling_times.end() };
	std::vector<double> falling_times_variances(SELECTION_FIRST_TEST_SIZE);
	for (double& m : falling_times_variances)
	{
		RaindropFall::test_falling(DROP_TEST_COUNT, INITIAL_HEIGHT, P_DOWN, P_UP, P_LEFT, P_RIGHT, falling_times);
		falling_times_double.assign(start_it, end_it);
		m = MathFunctions::variance(falling_times_double);
	}

	// определение минимально необходимого размера выборки и проведение дополнительных итераций по необходимости
	const double ESTIMATED_VARIANCE{ MathFunctions::math_expectation(falling_times_variances) };
	const double Q{ ACCURACY / ESTIMATED_VARIANCE };
	const size_t MIN_SELECTION_SIZE{ static_cast<size_t>(1 + 2 * pow(QUANTILE_INFINITY_0_95 / Q, 2)) };
	if (MIN_SELECTION_SIZE > SELECTION_FIRST_TEST_SIZE)
	{
		falling_times_variances.resize(MIN_SELECTION_SIZE);
		for (size_t i{ SELECTION_FIRST_TEST_SIZE }; i < falling_times_variances.size(); ++i)
		{
			RaindropFall::test_falling(DROP_TEST_COUNT, INITIAL_HEIGHT, P_DOWN, P_UP, P_LEFT, P_RIGHT, falling_times);
			falling_times_double.assign(start_it, end_it);
			falling_times_variances[i] = MathFunctions::variance(falling_times_double);
		}
	}

	const double MATH_EXPECTATION_FOR_VARIANCES{ MathFunctions::math_expectation(falling_times_variances) };
	const double VARIANCE_FOR_VARIANCES{ MathFunctions::variance(falling_times_variances, &MATH_EXPECTATION_FOR_VARIANCES) * SELECTION_FIRST_TEST_SIZE / (SELECTION_FIRST_TEST_SIZE - 1) };

	std::cout << "Заданный уровень надёжности: 0.95\n";
	std::cout << std::format("Заданная точность: {}\n", ACCURACY);
	std::cout << std::format("Размер выборки в пробном эксперименте: {}\n", SELECTION_FIRST_TEST_SIZE);
	std::cout << std::format("Оценка дисперсии: {}\n", ESTIMATED_VARIANCE);
	std::cout << std::format("Минимально необходимый размер выборки: {}\n\n", MIN_SELECTION_SIZE);

	std::cout << std::format("Математическое ожидание выборочной дисперсии после выполнения эксперимента:\n{}\n", MATH_EXPECTATION_FOR_VARIANCES);
	std::cout << std::format("Дисперсия выборочных дисперсий после выполнения эксперимента:\n{}\n", VARIANCE_FOR_VARIANCES);
	std::cout << std::format("Доверительный интервал:\n[{}, {}]\n", MATH_EXPECTATION_FOR_VARIANCES - ACCURACY, MATH_EXPECTATION_FOR_VARIANCES + ACCURACY);

	// график
	auto min_max{ std::minmax_element(falling_times_variances.begin(), falling_times_variances.end()) };
	auto histogram_graph{ matplot::hist(falling_times_variances) };
	histogram_graph->normalization(matplot::histogram::normalization::pdf);
	matplot::hold(true);
	auto approximate_normal_distribution_graph{ matplot::fplot(
		[MATH_EXPECTATION_FOR_VARIANCES, VARIANCE_FOR_VARIANCES](double x) -> double
		{
			return MathFunctions::normal_probability_density_function(x, MATH_EXPECTATION_FOR_VARIANCES, VARIANCE_FOR_VARIANCES);
		}, std::array<double, 2>{*min_max.first,* min_max.second}) };
	approximate_normal_distribution_graph->line_width(3);
}

void lab8_program()
{
	lab8_math_expectation();
	matplot::figure();
	lab8_variance();
	matplot::show();
}

void lab9_program()
{
	// TODO
	constexpr int WORKERS_INTENSITY{ 40 };
	constexpr int CASHIER_PERFORMANCE{ 30 };
	constexpr size_t CASHIER_COUNT{ 2 };

	const std::function<double()> request_time_generator{ []() -> double {return MathFunctions::exponential_random_value(WORKERS_INTENSITY); } };
	const std::function<double()> service_time_generator{ []() -> double {return MathFunctions::exponential_random_value(CASHIER_PERFORMANCE); } };

	QueueingSystem accounting{ CASHIER_COUNT, request_time_generator, service_time_generator };
	accounting.run(8);
}
