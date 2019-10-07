
// Fibonacci number
// Count fibonacci number in recrusive way and non-recursive way (dynamic programming)

#include <iostream>
#include <cstdint>

int64_t fib_recursive(unsigned int n)
{
	if ( 0 == n )
		return 0;
	
	if ( 1 == n )
		return 1;

	if ( 2 == n )
		return 2;

	return fib_recursive(n-1) + fib_recursive(n-2);
}

int64_t fib_nonrecursive(unsigned int n)
{
	int64_t today = 0; // result of fib n
	int64_t yesterday = 0; // cache of fib n-1
	int64_t day_before_yesterday = 0; // cache of fib n-2
	
	// set step one
	today = 1;
	yesterday = 1;
	day_before_yesterday = 1;

	for (int i = 1; i<n; ++i)
	{
		today = yesterday + day_before_yesterday;
		day_before_yesterday = yesterday;
		yesterday = today;
		
	}

	return today;
}


int main()
{
	for ( unsigned int n=1; n<20; ++n)
	{
		//int64_t fib_rec = fib_recursive(n);
		std::cout << "fib(" << n << "): " << fib_recursive(n) << " | " << fib_nonrecursive(n)  << std::endl;
	}
}
