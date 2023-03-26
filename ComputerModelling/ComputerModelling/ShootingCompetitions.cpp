#include "ShootingCompetitions.h"
#include "CoveyouGenerator.h"

double ShootingCompetitions::test_shooting(int tests_count, int bullets_count, const std::vector<double>& hit_probabilities, size_t index_for_tracking, int rand_seed)
{
	srand(rand_seed);
	std::vector<int> bullets(hit_probabilities.size(), bullets_count);
	size_t rest_bullets{ bullets_count * hit_probabilities.size() };
	size_t win_count{ 0 };
	bool is_hit{ false };
	for (size_t i{ 0 }; i < tests_count; ++i)
	{
		while (rest_bullets > 0 && !is_hit)
		{
			for (size_t j{ 0 }; j < hit_probabilities.size(); ++j)
			{
				if (bullets[j] > 0)
				{
					is_hit = (double)rand() / RAND_MAX <= hit_probabilities[j];
					if (is_hit)
					{
						if (j == index_for_tracking)
							++win_count;
						break;
					}
					--bullets[j];
					--rest_bullets;
				}
			}
		}
		bullets.assign(hit_probabilities.size(), bullets_count);
		rest_bullets = bullets_count * hit_probabilities.size();
		is_hit = false;
	}
	return (double)win_count / tests_count;
}

double ShootingCompetitions::test_shooting_by_mathematical(int bullets_count, const std::vector<double>& hit_probabilities, size_t index_for_tracking)
{
	double winning_probability{ 0 };

	double all_previous_miss_probability{ 1 };
	for (size_t i{ 0 }; i < index_for_tracking; ++i)
		all_previous_miss_probability *= 1 - hit_probabilities[i];
	double tracked_and_all_next_miss_probability{ 1 };
	for (size_t i{ index_for_tracking }; i < hit_probabilities.size(); ++i)
		tracked_and_all_next_miss_probability *= 1 - hit_probabilities[i];

	for (size_t i{ 1 }; i <= bullets_count; ++i)
	{
		winning_probability += pow(all_previous_miss_probability, i) * hit_probabilities[index_for_tracking] * pow(tracked_and_all_next_miss_probability, i - 1);
	}
	return winning_probability;
}
