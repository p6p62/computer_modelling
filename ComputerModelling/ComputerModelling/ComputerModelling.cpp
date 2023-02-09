#include <iostream>
#include "CoveyouGenerator.h"
#include "MathFunctions.h"

int main()
{
	constexpr int ELEMENTS_COUNT{ 5000 };
	constexpr int HISTOGRAM_PARTS_COUNT{ 12 };

	constexpr long SEED{ 6 };
	constexpr long DEGREE{ 9 };
	CoveyouGenerator generator{ SEED, DEGREE };

	std::vector<long> random_numbers;
	random_numbers.reserve(ELEMENTS_COUNT);
	for (size_t i = 0; i < ELEMENTS_COUNT; i++)
		random_numbers.push_back(generator.next());

	std::vector<int> histogram;
	MathFunctions::get_histogram(random_numbers, HISTOGRAM_PARTS_COUNT, histogram);
}
