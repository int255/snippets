
/**
 * Given a number n, count the '1' bits in its binary form
 */

#include <stdio.h>

int count_ones(unsigned int n)
{
	int ones = 0;
	while ( 0 < n)
	{
		ones += 1 & n;
		n >>= 1;
	}
	return ones;
}

int main()
{
	for (unsigned int n = 0; n<65535; ++n)
	{
		printf("count_ones(%d): %d\n", n, count_ones(n));
	}
	return 0;
}
