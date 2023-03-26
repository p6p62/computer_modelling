#pragma once
#include <vector>

class ShootingCompetitions
{
public:
	/// <summary>
	/// Тест стрельбы по мишеням
	/// </summary>
	/// <param name="tests_count">Количество повторений теста</param>
	/// <param name="bullets_count">Количество патронов у стрелков</param>
	/// <param name="hit_probabilities">Вероятности попадания</param>
	/// <param name="index_for_tracking">Индекс отслеживаемого стрелка</param>
	/// <param name="rand_seed">Начальное значение генератора случайных чисел</param>
	/// <returns>Вероятность, что выигрывает стрелок с индексом index_for_tracking</returns>
	static double test_shooting(int tests_count, int bullets_count, const std::vector<double>& hit_probabilities, size_t index_for_tracking, int rand_seed);
	static double test_shooting_by_mathematical(int bullets_count, const std::vector<double>& hit_probabilities, size_t index_for_tracking);
};

