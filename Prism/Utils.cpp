#include "Utils.h"

float clamp(float n, float lo, float hi)
{
	if (n < lo)
		return lo;
	else if (n > hi)
		return hi;

	return n;
}
