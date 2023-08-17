#pragma once
#include <Siv3D.hpp>

bool probability(double a)
{
	if (Random() <= a)
	{
		return true;
	}
	else
	{
		return false;
	}
}

