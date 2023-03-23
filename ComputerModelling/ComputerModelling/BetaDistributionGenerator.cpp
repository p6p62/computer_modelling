#include "BetaDistributionGenerator.h"

BetaDistributionGenerator::BetaDistributionGenerator(double p, double m, const std::function<double()> uniform_number_generator) : _p_parameter{ p }, _m_parameter{ m }, _uniform_number_generator{ uniform_number_generator }
{
}

double BetaDistributionGenerator::next()
{
	double uniform1{ _uniform_number_generator() };
	double uniform2{ _uniform_number_generator() };
	while (pow(uniform1, 1 / _p_parameter) + pow(uniform2, 1 / _m_parameter) > 1)
	{
		uniform1 = _uniform_number_generator();
		uniform2 = _uniform_number_generator();
	}
	return pow(uniform1, 1 / _p_parameter) / (pow(uniform1, 1 / _p_parameter) + pow(uniform2, 1 / _m_parameter));
}
