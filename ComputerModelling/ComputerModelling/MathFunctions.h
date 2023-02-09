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

		const float step{ (max - min) / (float)parts_count };
		histogram.clear();
		histogram.resize(parts_count);
		for (const T& v : data)
		{
			++histogram[std::min<size_t>((v - min) / step, parts_count - 1)];
		}
	}
};

