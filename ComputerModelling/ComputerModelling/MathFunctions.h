#pragma once
#include <vector>
#include <functional>

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

	static std::vector<double> separate_on_parts(unsigned int parts_count, double a = 0, double b = 1);

	static double moment_initial(const std::vector<double>& data, int moment_number);
	static double moment_central(const std::vector<double>& data, int moment_number, const double* math_expectation = nullptr);
	static double math_expectation(const std::vector<double>& data);
	static double variance(const std::vector<double>& data, const double* math_expectation = nullptr);

	static std::vector<double> get_empirical_distribution_function(const std::vector<double>& data, const std::vector<double>& intervals);
	static double uniform_distribution_function(double x, double a = 0, double b = 1);

	static double pearson_criteria(const std::vector<double>& data, const std::vector<double>& intervals, const std::function<double(double)>& theoretical_distribution_function);
};

