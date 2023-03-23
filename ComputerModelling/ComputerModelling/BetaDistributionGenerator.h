#pragma once
#include <functional>

class BetaDistributionGenerator
{
private:
	const double _p_parameter;
	const double _m_parameter;
	const std::function<double()> _uniform_number_generator;

public:
	BetaDistributionGenerator(double p, double m, const std::function<double()> uniform_number_generator);

public:
	double next();
};

