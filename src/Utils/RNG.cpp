#include "RNG.h"

RNG::RNG(unsigned int seed_value /*= std::time(0)*/)
{
	rng_.seed(seed_value);
	seed_value_ = seed_value;
}

RNG::~RNG()
{
}

unsigned int RNG::getSeed()
{
	return seed_value_;
}

int RNG::nextInt(int n1, int n2)
{
	std::uniform_int_distribution<rng_type_::result_type> udist(n1, n2);
	return udist(rng_);
}