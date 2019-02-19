#pragma once
#include"global.h"

_MATH_BEGIN

template<typename T>
static T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x);
}

_MATH_END