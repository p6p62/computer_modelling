#pragma once
class CoveyouGenerator
{
private:
	long _current_value;
	const long _degree;
	const long _delimeter;

public:
	CoveyouGenerator(long initial_seed, short degree_of_two);

	double next();
};

