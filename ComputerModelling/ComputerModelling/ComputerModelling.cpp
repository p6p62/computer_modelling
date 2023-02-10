#include <iostream>
#include "CoveyouGenerator.h"
#include "MathFunctions.h"
#include <algorithm>

void print_histogram(const std::vector<int>& histogram, int max_width = 70)
{
	constexpr int OFFSET{ 15 };
	size_t data_count{ 0 };
	for (int v : histogram)
		data_count += v;
	for (int value : histogram)
	{
		std::string s;
		s = "|" + std::string(static_cast<size_t>((max_width - OFFSET) * ((double)value / data_count) * histogram.size()), '=');
		std::cout << s << ' ' << value << std::endl;
	}
}

int main()
{
	constexpr int ELEMENTS_COUNT{ 5000 };
	constexpr int HISTOGRAM_PARTS_COUNT{ 12 };

	constexpr long SEED{ 10 };
	constexpr long DEGREE{ 12 };

	CoveyouGenerator generator{ SEED, DEGREE };

	std::vector<double> random_numbers;
	random_numbers.reserve(ELEMENTS_COUNT);
	for (size_t i = 0; i < ELEMENTS_COUNT; i++)
		random_numbers.push_back(generator.next());

	std::vector<int> histogram;
	MathFunctions::get_histogram(random_numbers, HISTOGRAM_PARTS_COUNT, histogram);
	print_histogram(histogram);
}
