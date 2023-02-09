#include "CoveyouGenerator.h"
#include <stdexcept>

CoveyouGenerator::CoveyouGenerator(long initial_seed, short degree_of_two) : _current_value{ initial_seed }, _degree{ 1 << degree_of_two }
{
	if (initial_seed % 4 != 2)
		throw std::invalid_argument("������, ������ ����������� initial_seed % 4 == 2");
}

long CoveyouGenerator::next()
{
	_current_value = (_current_value * (_current_value + 1)) % _degree;
	return _current_value;
}
