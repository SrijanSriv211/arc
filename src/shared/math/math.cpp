#include "arcpch.h"
#include "math.h"

namespace math
{
	// there's a subtle but important difference in how the modulo operator (%) handles negative numbers in C++ compared to Python
	// in C++: `7 % 2 = 1`, in Python: `7 % 2 = 1`
	// in C++: `-7 % 2 = -1`, in Python: `-7 % 2 = 1`
	// this function handles modulo in C++ just like in Python
	int modulo(const int& dividend, const int& divisor)
	{
		return (dividend % divisor + divisor) % divisor;
	}
}
