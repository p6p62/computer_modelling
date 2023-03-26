#pragma once
#include <vector>

class ShootingCompetitions
{
public:
	/// <summary>
	/// ���� �������� �� �������
	/// </summary>
	/// <param name="tests_count">���������� ���������� �����</param>
	/// <param name="bullets_count">���������� �������� � ��������</param>
	/// <param name="hit_probabilities">����������� ���������</param>
	/// <param name="index_for_tracking">������ �������������� �������</param>
	/// <param name="rand_seed">��������� �������� ���������� ��������� �����</param>
	/// <returns>�����������, ��� ���������� ������� � �������� index_for_tracking</returns>
	static double test_shooting(int tests_count, int bullets_count, const std::vector<double>& hit_probabilities, size_t index_for_tracking, int rand_seed);
	static double test_shooting_by_mathematical(int bullets_count, const std::vector<double>& hit_probabilities, size_t index_for_tracking);
};

