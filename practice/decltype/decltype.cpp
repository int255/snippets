#include <iostream>


decltype(auto) f(int a)
{
	int x = a;
	++x;
	return x;
}

int main()
{
	std::cout << "type: " << typeid(decltype(f(1))).name() << std::endl;
	for ( int i = 0; i<5; ++i)
	{
		std::cout << "f(" << i << "): " << f(i) << std::endl;
	}
	return 0;
}
