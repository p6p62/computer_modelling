#pragma once
#include <functional>
#include <vector>

class QueueingSystem
{
public:
	struct WorkMetrics
	{
		double load_factor{ 0 };
		double probability_accept{ 1 };
		double average_queue_fill{ 0 };
		double average_service_fill{ 0 };
		double average_single_service_time{ 0 };
		double average_system_spent_time{ 0 };
	};

private:
	size_t _channels{ 1 };
	std::function<double()> _request_time_generator;
	std::function<double()> _service_time_generator;

	std::vector<double> _requests_times;
	std::vector<double> _servicing_times;

private:
	static void generate_event_times(double working_time, std::vector<double>& data, const std::function<double()>& time_generator);

public:
	QueueingSystem(size_t channels, const std::function<double()>& request_time_generator, const std::function<double()>& service_time_generator);

public:
	WorkMetrics run(double working_time);
};

