#pragma once
class CoveyouGenerator
{
private:
	long _current_value;
	const long _degree;

public:
	CoveyouGenerator(long initial_seed, short degree_of_two);

	long next();
};

