#pragma once
#include <vector>

class MathFunctions
{
public:
	template <typename T>
	static void get_histogram(const std::vector<T>& data, unsigned int parts_count, std::vector<int>& histogram)
	{
		if (data.empty())
			return;

		T min{ data[0] };
		T max{ data[0] };
		for (const T& v : data)
		{
			if (v < min)
				min = v;
			else if (v > max)
				max = v;
		}

		const double step{ (max - min) / (double)parts_count };
		histogram.clear();
		histogram.resize(parts_count);
		for (const T& v : data)
		{
			++histogram[std::min<size_t>(static_cast<size_t>((v - min) / step), static_cast<size_t>(parts_count - 1))];
		}
	}
};

