#pragma once
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

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

	template <typename T, typename T1>
	static void get_histogram_on_custom_intervals(const std::vector<T>& data, const std::vector<double>& intervals, std::vector<T1>& histogram)
	{
		if (data.empty() || intervals.size() < 2)
			return;

		std::vector<T> sorted_data(data);
		std::sort(sorted_data.begin(), sorted_data.end());

		histogram.clear();
		histogram.resize(intervals.size() - 1);
		for (size_t i{ 1 }, data_counter{ 0 }; i < intervals.size(); ++i)
		{
			const double right_border{ intervals[i] };
			constexpr double ACCURATION{ 0.000001 };
			while (data_counter < sorted_data.size() && sorted_data[data_counter] < right_border + ACCURATION)
			{
				++histogram[i - 1];
				++data_counter;
			}
		}
	}

	static std::vector<double> separate_on_parts(unsigned int parts_count, double a = 0, double b = 1);

	static double moment_initial(const std::vector<double>& data, int moment_number);
	static double moment_central(const std::vector<double>& data, int moment_number, const double* math_expectation = nullptr);
	static double math_expectation(const std::vector<double>& data);
	static double variance(const std::vector<double>& data, const double* math_expectation = nullptr);

	static double combination(unsigned long m, unsigned long n);

	/// <summary>
	/// ��������� ������� �������������, ������������ ��� ������ ����������
	/// </summary>
	/// <param name="data">������</param>
	/// <param name="intervals">����� ����������</param>
	/// <returns>������������ ������� �������������</returns>
	static std::vector<double> get_empirical_distribution_function(const std::vector<double>& data, const std::vector<double>& intervals);

	/// <summary>
	/// ��������� ������� �������������, ��������������� � ���� ������������ ��� ��������������� ����
	/// </summary>
	/// <param name="data">������</param>
	/// <param name="out_values">�������� ��������. ����� ���� [�������� ������� �������������-��������]</param>
	static void get_statistical_distribution_function_points(const std::vector<double>& data, std::map<double, double>& out_values);
	static double uniform_distribution_function(double x, double a = 0, double b = 1);

	/// <summary>
	/// ������ �������� ��-������� �� ������ ������
	/// </summary>
	/// <param name="data">������ ��� �������</param>
	/// <param name="intervals">�����-������� ����������. ������ ������� - ������ ������� ���������, ������ - ����� ������� ���������/������ ������� ���������, ������ - ����� ������� ���������..., ��������� - ����� ���������� ���������</param>
	/// <param name="theoretical_distribution_function">������������� ������� ������������� �����������, � ������� ������� ����� ������������� ���������</param>
	/// <returns>�������� ��-�������</returns>
	static double pearson_criteria(const std::vector<double>& data, const std::vector<double>& intervals, const std::function<double(double)>& theoretical_distribution_function);

	/// <summary>
	/// ������ �������� �������� ����������� �� ������ ������
	/// </summary>
	/// <param name="data">������ ��� �������</param>
	/// <param name="theoretical_distribution_function">������������� ������� �������������</param>
	/// <returns>�������� �������� �����������</returns>
	static double colmogorov_criteria(const std::vector<double>& data, const std::function<double(double)>& theoretical_distribution_function);

	static double collector_criteria(const std::vector<double>& data, unsigned int digit_count);

	static double normal_probability_density_function(double x, double math_expectation, double variance);

	static double normal_distribution_function(double x, double math_expectation, double variance);

	static double gamma_probability_density_function(double x, double k_parameter, double theta_parameter);
};
