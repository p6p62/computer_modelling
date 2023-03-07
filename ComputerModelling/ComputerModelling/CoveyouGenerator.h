#pragma once
class CoveyouGenerator
{
public:
	static constexpr long SEED_DEFAULT{ 10 };
	static constexpr int DEGREE_DEFAULT{ 12 };

private:
	long _current_value;
	const long _degree;
	const long _delimeter;

public:
	CoveyouGenerator(long initial_seed, short degree_of_two);
	CoveyouGenerator();

	double next();
};

