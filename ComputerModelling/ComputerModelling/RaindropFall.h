#pragma once
#include <vector>

class RaindropFall
{
public:
	static void test_falling(size_t tests_count, int initial_height, double p_down, double p_up, double p_left, double p_right, std::vector<long>& falling_times);
};

