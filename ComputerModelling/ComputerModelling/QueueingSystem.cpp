#include "QueueingSystem.h"

void QueueingSystem::generate_event_times(double working_time, std::vector<double>& data, const std::function<double()>& time_generator)
{
	double common_time{ 0 };
	while (true)
	{
		common_time += time_generator();
		if (common_time <= working_time)
			data.push_back(common_time);
		else
			break;
	}
}

QueueingSystem::QueueingSystem(size_t channels, const std::function<double()>& request_time_generator, const std::function<double()>& service_time_generator)
	: _channels{ channels }, _request_time_generator{ request_time_generator }, _service_time_generator{ service_time_generator }
{
}

QueueingSystem::WorkMetrics QueueingSystem::run(double working_time)
{
	// создание данных о времени поступления заявок и их обслуживания
	generate_event_times(working_time, _requests_times, _request_time_generator);
	for (size_t i{ 0 }; i < _requests_times.size(); ++i)
		_servicing_times.push_back(_service_time_generator());

	// выполнение заявок
	double model_time{ 0 };
	for (size_t i{ 0 }; i < _requests_times.size(); ++i)
	{

	}

	return WorkMetrics();
}
