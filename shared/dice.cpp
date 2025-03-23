#include "stdafx.h"
#include "dice.h"
#include <time.h>
#include <minwindef.h>
#include <random>

//create a random generator engine
// using current timestamp as the seed
std::mt19937 Dice::generator = std::mt19937( (unsigned int)time(0));

short Dice::RandomS(short l, short h)
{
	if (l == h)
		return l;

	std::uniform_int_distribution<short> distribution(l, h);
	return distribution(Dice::generator);
}

//Between two values (inclusive!)
int Dice::RandomI(int l, int h)
{
	if (l == h)
		return l;

	std::uniform_int_distribution<int> distribution(l, h);
	return distribution(Dice::generator);
}

//between two values (inclusive!)
UINT Dice::RandomU(UINT l, UINT h)
{
	if (l == h)
		return l;

	std::uniform_int_distribution<UINT> distribution(l, h);
	return distribution(Dice::generator);
}

//between two values (inclusive!)
float Dice::RandomF(float l, float h)
{
	if (l == h)
		return l;

	std::uniform_real_distribution<float> distribution(l, h);
	return distribution(Dice::generator);
}

//between two values (inclusive!)
double Dice::RandomD(double l, double h)
{
	if (l == h)
		return l;

	std::uniform_real_distribution<double> distribution(l, h);
	return distribution(Dice::generator);
}