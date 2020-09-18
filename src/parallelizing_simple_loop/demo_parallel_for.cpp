#include <iostream>
#include "tbb/tbb.h"
#include "common/chrono_time.h"

using namespace tbb;

static inline void Foo(float& f) {
    f += 1.5f;
}

static void SerialApplyFoo( float a[], size_t n ) {
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
    ApplyFoo( float a[] ) :
        my_a(a)
    {}
};

static void ParallelApplyFoo( float a[], size_t n ) {
    parallel_for(blocked_range<size_t>(0,n), ApplyFoo(a));
}

int main(int argc, char* argv[]) {
    static const int arr_size = 409600;
    float arr[arr_size] = { 0.f };

    ChronoTime timer;
    timer.Start();
    SerialApplyFoo(arr, arr_size);
    timer.Stop();
    std::cout << "SerialApplyFoo time:" << timer.GetTime() << " ms." << std::endl;

    timer.Start();
    ParallelApplyFoo(arr, arr_size);
    timer.Stop();
    std::cout << "ParallelApplyFoo time:" << timer.GetTime() << " ms." << std::endl;

    return 0;
}
