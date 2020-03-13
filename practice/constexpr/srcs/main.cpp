#include <iostream>

#include <array>


template<int N>
struct MyArray {
    std::array<int,N> value;
    constexpr MyArray() : value(){
        for (auto i = 0; i != N; ++i)
            value[i] = i;
    }
    
};



constexpr size_t numGridRows(size_t N)
{
	switch (N)
	{
		case 4:
			return 2;
		case 6: 
			return 3;
		case 9:
			return 3;
		default:
			break;
	}
	return 0;
}

int main()
{
	constexpr auto numRows = numGridRows(9);
    std::array<int, numGridRows(9)> arr;
	std::cout << "Num rows: " << arr.size() << std::endl;
    
    MyArray<5> myarr;
	return 0;
}
