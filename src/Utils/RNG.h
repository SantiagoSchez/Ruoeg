//
// Just a wrapper for <random> library
// 

#ifndef RUOEG_UTILS_RNG_H_
#define RUOEG_UTILS_RNG_H_

#include <ctime>
#include <random>

class RNG
{
public:
	explicit RNG(unsigned int seed_value = std::time(0));
	~RNG();

	unsigned int getSeed();
	int nextInt(int n1, int n2);

private:
	typedef std::mt19937 rng_type_;
	rng_type_ rng_;
	unsigned int seed_value_;
};

#endif // RUOEG_UTILS_RNG_H_