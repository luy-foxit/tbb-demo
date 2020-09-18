#include <iostream>
#include <stdlib.h>
#include "tbb/parallel_for.h"
#include "common/tbb_time.h"
#include "common/chrono_time.h"

using namespace tbb;

static inline void Foo(float& f) {
    f = static_cast<float>(rand()*1.0f/RAND_MAX);
    //f = 1.f;
}

static void SerialApplyFoo( float* a, size_t n ) {
    for( size_t i=0; i!=n; ++i )
        Foo(a[i]);
}

class ApplyFoo {
    float *const my_a;
public:
    void operator()( const blocked_range<size_t>& r ) const {
        float *a = my_a;
        for( size_t i=r.begin(); i!=r.end(); ++i ) 
            Foo(a[i]);
    }
    ApplyFoo( float* a ) :
        my_a(a)
    {}
};

static void ParallelApplyFoo( float* a, size_t n ) {
    parallel_for(blocked_range<size_t>(0,n), ApplyFoo(a));
}

static void ParallelApplyFooLambda( float* a, size_t n ) {
    parallel_for( blocked_range<size_t>(0,n), 
            [=](const blocked_range<size_t>& r) {
            for(size_t i=r.begin(); i!=r.end(); ++i) 
            Foo(a[i]); 
            }
    );
}

int main(int argc, char* argv[]) {
    static const int arr_size = 4096000;
    float* arr1 = new float[arr_size];
    float* arr2 = new float[arr_size];
    float* arr3 = new float[arr_size];

    //ChronoTime timer1;
    TbbTime timer;
    timer.Start();
    SerialApplyFoo(arr1, arr_size);
    timer.Stop();
    std::cout << "SerialApplyFoo time:" << timer.GetTime() << " ms." << std::endl;
    std::cout << "arr1[15]:" << arr1[15] << std::endl;

    timer.Start();
    ParallelApplyFoo(arr2, arr_size);
    timer.Stop();
    std::cout << "ParallelApplyFoo time:" << timer.GetTime() << " ms." << std::endl;
    std::cout << "arr2[15]:" << arr2[15] << std::endl;

    timer.Start();
    ParallelApplyFooLambda(arr3, arr_size);
    timer.Stop();
    std::cout << "ParallelApplyFooLambda time:" << timer.GetTime() << " ms." << std::endl;
    std::cout << "arr3[15]:" << arr3[15] << std::endl;

    delete[] arr1;
    delete[] arr2;
    delete[] arr3;

    return 0;
}
