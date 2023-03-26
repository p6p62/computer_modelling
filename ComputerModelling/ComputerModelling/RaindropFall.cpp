#include "RaindropFall.h"
#include "CoveyouGenerator.h"

void RaindropFall::test_falling(size_t tests_count, int initial_height, double p_down, double p_up, double p_left, double p_right, std::vector<long>& falling_times)
{
	const double UP_INTERVAL{ p_down + p_up };
	//CoveyouGenerator random;
	falling_times.resize(tests_count);
	for (long& i : falling_times)
	{
		int height{ initial_height };
		long time{ 0 };
		while (height > 0)
		{
			++time;
			//double move_selector{ random.next() };
			double move_selector{ (double)rand() / RAND_MAX };
			if (move_selector <= p_down)
			{
				--height;
			}
			else if (move_selector <= UP_INTERVAL)
			{
				++height;
			}
		}
		i = time;
	}
}
