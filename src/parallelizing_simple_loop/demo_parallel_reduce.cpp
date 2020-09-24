#include <iostream>
#include <stdlib.h>
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include "common/tbb_time.h"
#include "common/chrono_time.h"

using namespace tbb;

static inline float Foo(float f) {
    //f = static_cast<float>(rand()*1.0f/RAND_MAX);
    return f;
}

float SerialSumFoo( float* a, size_t n ) {
    float sum = 0;
    for( size_t i=0; i!=n; ++i )
        sum += Foo(a[i]);
    return sum;
}

class SumFoo {
    float* my_a;
public:
    float my_sum; 
    void operator()( const blocked_range<size_t>& r ) {
        float *a = my_a;
        float sum = my_sum;
        size_t end = r.end();
        for( size_t i=r.begin(); i!=end; ++i ) 
            sum += Foo(a[i]); 
        my_sum = sum;
    }
 
    SumFoo( SumFoo& x, split ) : my_a(x.my_a), my_sum(0) {}
 
    void join( const SumFoo& y ) {my_sum+=y.my_sum;}
             
    SumFoo(float* a ) :
        my_a(a), my_sum(0)
    {}
};

float ParallelSumFoo( float* a, size_t n ) {
    SumFoo sf(a);
    parallel_reduce( blocked_range<size_t>(0,n), sf );
    return sf.my_sum;
}

int main(int argc, char *argv[])
{
    static const int arr_size = 4096000;

    float *arr = new float[arr_size];
    for (int i = 0; i < arr_size; ++i)
    {
        arr[i] = 1.f;
    }
    //ChronoTime timer1;
    TbbTime timer;
    {
        
        timer.Start();
        float result = SerialSumFoo(arr, arr_size);
        timer.Stop();
        std::cout << "SerialSumFoo time:" << timer.GetTime() << " ms. result:" << result << std::endl;
    }

    {
        timer.Start();
        float result = ParallelSumFoo(arr, arr_size);
        timer.Stop();
        std::cout << "ParallelSumFoo time:" << timer.GetTime() << " ms. result:" << result << std::endl;
    }

    return 0;
}
