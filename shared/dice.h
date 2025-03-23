#pragma once

#include <random>
#include <minwindef.h>

class Dice
{
public:
	static std::mt19937	generator;
	static short RandomS(short l, short h);
	static int RandomI(int l, int h);
	static UINT RandomU(UINT l, UINT h);
	static float RandomF(float l, float h);
	static double RandomD(double l, double h);
};