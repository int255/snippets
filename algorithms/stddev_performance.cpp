#include <numeric>
#include <random>
#include <iostream>
#include <type_traits>

namespace {
	using real = float;
    // Tracking -> penalty lookup table
    template <int N>
    struct PenaltyLUT
    {
        PenaltyLUT()
        {
            _penalties.resize(N);
            for ( auto i=0; i<N; ++i)
            {
                _penalties[i] = calculatePenalty(i);
            }
            
            for (int i =0; i<N; ++i )
            {
                real tr = i*kInterval100;
                _penaltiesPer100.push_back(calculatePenalty(tr));
            }
        }

        real penalty(real tracking) const
        {
            assert(tracking >=0.0);
            if (tracking < N )
            {
                return _penalties[(int)tracking];
            } else {
                int index = tracking/kInterval100;
                if ((index+1)<_penaltiesPer100.size())
                {
                    auto y0 = _penaltiesPer100[index];
                    auto y1 = _penaltiesPer100[index+1];
                    assert(y1 > y0);
                    auto x0 = index*kInterval100;
                    auto x1 = (index+1)*kInterval100;
                    assert(x1 > x0);
                    assert( tracking >= x0 && tracking <= x1);
                    auto penalty = y0 + (y1 - y0) * (tracking - x0) / (x1 - x0);
                    return penalty;
                } else {
                    
                    auto penalty = calculatePenalty(tracking);
                    return penalty;
                }
            }
        }
        
    private:
        static constexpr real kInterval100 = 100.0;
        std::vector<real> _penalties;
        std::vector<real> _penaltiesPer100;
        real calculatePenalty(real tracking) const
        {
            static const real kBigTracking = 5000.0;
            static const real deno = std::log10(kBigTracking);
            static const real factor = 1.0/deno;
            return std::log10(1.0+tracking)*factor;
        }
        
    };
    
    
    // Transform a given tracking value to a normalized penalty value between 0.0 ... 1.0
    // Noticd in Extreme case, e.g. tracking > 5000.0, penalty value can still exceeds 1.0
    real trackingPenalty(real tracking)
    {
        static const auto lut = PenaltyLUT<2000>();
        return lut.penalty(tracking);
    }
}
std::vector<real> generateData(size_t N)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 4000); // distribution in range [0, 2000]

	std::vector<real> data;
	for (size_t i=0; i<N; ++i)
	{
		auto n = dist(gen);
		if ( n < 3000)
		{
			data.push_back(0.0);
		} else {
			data.push_back((n - 3000));
		}
	}

	return data;
}

namespace {
  template <typename T>
    struct counter_t
    {
        T value;
        size_t occurence;
    };
    
    template <typename T>
    bool areClose(const T & a, const T & b, const T & tolerance)
    {
        return std::fabs(a-b) <= tolerance;
    }
    
    template <typename T>
    T mostFrequentElement(const std::vector<T> & inputs, const T & tolerance)
    {
 		
        std::vector<counter_t<T>> counters;
        for ( auto & e : inputs)
        {
            auto it = std::find_if(counters.begin(), counters.end(), [&e, tolerance](auto & counter){
                return areClose(counter.value, e, tolerance /* per thousand Em tracking */);
            });
            if (it == counters.end())
            {
                counter_t<T> c;
                c.value = e;
                c.occurence = 1;
                counters.push_back(c);
            } else {
                ++it->occurence;
            }
        }
        
        auto it = std::max_element(counters.begin(), counters.end(), [](auto & a, auto & b){ return a.occurence < b.occurence; });
        assert(it!=counters.end());
        return it->value;
    }

    template <typename T>
    T findMode(const std::vector<T> & inputs)
    {
    	static_assert( std::is_integral<T>::value, "OK");
    	static_assert( std::is_unsigned<T>::value, "OK");
    	static const size_t N = 1000;
    	static std::vector<unsigned int> counters(N+1);
    	memset(counters.data(), 0, counters.size() * sizeof(counters[0]));
		
        //std::vector<counter_t<T>> counters;
        for ( auto & e : inputs)
        {
        	if ( e < N)
        	{
        		++counters[e];
        	}
        }
        
        auto it = std::max_element(counters.begin(), counters.end(), [](auto a, auto b){ return a<b; });
        assert(it!=counters.end());
        return *it;
    }
    // Standard deviation with arbitary mean
    template <typename T>
    T standardDeviation(const std::vector<T> & inputs, T mean)
    {
        if (inputs.empty())
            return 0;
        
        T variance = 0;
        for ( auto & e : inputs)
        {
            auto deviation = e - mean;
            variance += deviation*deviation;
        }
        
        T stddev = std::sqrt( (variance)/inputs.size() );
        return stddev;
    }


    // Standard deviation with arbitary mean
    template <typename T>
    T standardDeviation_zero_optimized(const std::vector<T> & inputs, T mean)
    {
        if (mean == 0)
        {
        	if (inputs.empty())
            	return 0.0;	

            T variance = 0;
            std::for_each(inputs.begin(), inputs.end(), [&variance](auto e){ 
            	if (e!=0)
            	{
            		variance += (e*e);
            	}
			});
            
            return std::sqrt( (variance)/inputs.size() );
        } else {
			return standardDeviation(inputs, mean);
        }
		
    }

    // Standard deviation with arbitary mean
    template <typename T>
    T absoluteDeviation(const std::vector<T> & inputs, T mean)
    {
    	static_assert(std::is_integral<T>::value, "must be int");
        if (inputs.empty())
            return 0.0;
        
        T variance = 0.0;
        for ( auto & e : inputs)
        {
            variance += std::abs((int)e - (int)mean);
        }        
        T stddev = variance/inputs.size();
        return stddev;
    }    
}


real stddev_imp1(const std::vector<real> v)
{
	auto mode = mostFrequentElement(v, 0.001f);
	auto stddev = standardDeviation(v, mode);
	return stddev;
}

real stddev_imp2(const std::vector<real> v)
{
	std::vector<int> iv;
	iv.resize(v.size());
	std::transform(v.begin(), v.end(), iv.begin(), [](auto e){ return 1000*e;});
	auto mode = mostFrequentElement(iv, 1);
	auto stddev = standardDeviation(iv, mode);
	return 0.001 *stddev;
}

real stddev_imp3(const std::vector<real> v)
{
	std::vector<unsigned int> iv;
	iv.resize(v.size());
	std::transform(v.begin(), v.end(), iv.begin(), [](auto e){ return 1000*e;});
	auto mode = findMode(iv);
	//int mode = 0;
	auto stddev = standardDeviation(iv, mode);
	return 0.001 *stddev;
}

real stddev_imp4(const std::vector<real> & v)
{
	std::vector<unsigned int> iv;
	iv.resize(v.size());
	std::transform(v.begin(), v.end(), iv.begin(), [](auto e){ return 1000*e;});
	auto mode = findMode(iv);
	//int mode = 0;
	//auto stddev = standardDeviation(iv,mode);//_zero_optimized(iv, mode);
	auto stddev = standardDeviation_zero_optimized(iv, mode);
	return 0.001 *stddev;
}

real stddev_imp5(const std::vector<real> & v)
{
	std::vector<unsigned int> iv;
	iv.reserve(v.size());
	//std::transform(v.begin(), v.end(), iv.begin(), [](auto e){ return 1000*e;});
	int counter=0;
	for ( auto it = v.begin(); it!=v.end(); ++it)
	{
		if ( *it != 0.0)
		{
			iv.push_back(1000*(*it));
			++counter;
		}
	}

	iv.resize(counter);
	std::cout << "v.size() : iv.size() " << v.size() << " : " << iv.size() << std::endl;
	
	assert( counter*2 < v.size());
	unsigned int mode = 0;
	int variance = 0;
	std::for_each(iv.begin(), iv.end(), [&variance](auto e){ variance += e*e;});
	
	//int mode = 0;
	auto stddev = sqrt(variance/v.size());//standardDeviation(iv, mode);
	return 0.001 *stddev;
}

real absdev_imp1(const std::vector<real> v)
{
	std::vector<unsigned int> iv;
	iv.resize(v.size());
	std::transform(v.begin(), v.end(), iv.begin(), [](auto e){ return 1000*e;});
	auto mode = findMode(iv);
	auto stddev = absoluteDeviation(iv, mode);
	return 0.001 *stddev;
}

template <class Lambda>
void time_evaluate(const std::string & name, Lambda lambda)
{
	auto start = std::chrono::system_clock::now();
	// do some work
	lambda();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<real, std::milli> diff = end-start;
	std::cout << "Time evaluate " << name << " : " << diff.count() << " ms\n"; 
}

int main()
{
	srand(time(0));
	
	std::vector<real> v;
	time_evaluate("generateData", [&v](){ v = generateData(1000000);});
	
	/*
	std::for_each(v.begin(), v.end(), [](auto e){
		std::cout << e << std::endl;
	});*/
	trackingPenalty(1);

	time_evaluate("trackingPenalty", [&v](){
		std::transform(v.begin(), v.end(), v.begin(), [](auto e){
			return trackingPenalty(e);
		});
	});
	time_evaluate("stddev_imp1", [&v](){
		auto stddev = stddev_imp1(v);
		std::cout << "stddev: " << stddev << std::endl;
	});

	time_evaluate("stddev_imp2", [&v](){
		auto stddev = stddev_imp2(v);
		std::cout << "stddev: " << stddev << std::endl;
	});

	time_evaluate("stddev_imp3", [&v](){
		auto stddev = stddev_imp3(v);
		std::cout << "stddev: " << stddev << std::endl;
	});

	time_evaluate("stddev_imp4", [&v](){
		auto stddev = stddev_imp4(v);
		std::cout << "stddev: " << stddev << std::endl;
	});

	time_evaluate("stddev_imp5", [&v](){
		auto stddev = stddev_imp5(v);
		std::cout << "stddev: " << stddev << std::endl;
	});

	time_evaluate("absdev_imp1", [&v](){
		auto absdev = absdev_imp1(v);
		std::cout << "absdev: " << absdev << std::endl;
	});

	return 0;
}
