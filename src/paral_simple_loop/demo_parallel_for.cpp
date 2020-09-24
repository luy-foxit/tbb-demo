#include <iostream>
#include <stdlib.h>
#include "tbb/parallel_for.h"
#include "common/tbb_time.h"
#include "common/chrono_time.h"

using namespace tbb;

static inline void Foo(float& f, size_t data) {
    //f = static_cast<float>(rand()*1.0f/RAND_MAX);
    f = static_cast<float>(data);
}

static void SerialApplyFoo( float* a, size_t n ) {
    for( size_t i=0; i!=n; ++i )
        Foo(a[i], i);
}

class ApplyFoo {
    float *const my_a;
public:
    void operator()( const blocked_range<size_t>& r ) const {
        float *a = my_a;
        for( size_t i=r.begin(); i!=r.end(); ++i ) 
            Foo(a[i], i);
    }
    ApplyFoo( float* a ) :
        my_a(a)
    {}
};

// auto chunking 
static void ParallelApplyFoo( float* a, size_t n ) {
    parallel_for(blocked_range<size_t>(0,n), ApplyFoo(a));
}

// auto chunking 
static void ParallelApplyFooLambda( float* a, size_t n ) {
    parallel_for( blocked_range<size_t>(0,n), 
            [=](const blocked_range<size_t>& r) {
            for(size_t i=r.begin(); i!=r.end(); ++i) 
            Foo(a[i], i); 
            }
    );
}

// control chunking 
static void ParallelApplyFooChunk( float* a, size_t n ) {
    // 使用 simple_partitioner() 作为 parallel_for 的第三个参数，用于关闭自动chunking
    // 构造函数的线程参数形式是blocked_range(begin,end,grainsize)。grainsize的默认值是1。它以每个块的循环迭代为单位。

    int G = 1000;
    parallel_for(blocked_range<size_t>(0,n,G), ApplyFoo(a), 
            simple_partitioner());
}

// 带管与缓存关联 (Bandwidth and Cache Affinity)
static void ParallelApplyFooAP( float* a, size_t n ) {
    // affinity_partitioner对象ap在循环迭代之间存在。
    // 它会记住循环的迭代运行的位置，因此每次迭代都可以传递给之前执行它的同一个线程。
    static affinity_partitioner ap;
    parallel_for(blocked_range<size_t>(0,n), ApplyFoo(a), ap);
}

static void TimeStepFoo( float* a, size_t n, int steps ) {    
    for( int t=0; t<steps; ++t )
        ParallelApplyFooAP( a, n );
}

int main(int argc, char *argv[])
{
    static const int arr_size = 409600;

    //ChronoTime timer1;
    TbbTime timer;
    {
        float *arr = new float[arr_size];
        timer.Start();
        SerialApplyFoo(arr, arr_size);
        timer.Stop();
        std::cout << "SerialApplyFoo time:" << timer.GetTime() << " ms." << std::endl;
        std::cout << "arr last data:" << arr[arr_size - 1] << std::endl;
        delete[] arr;
    }

    {
        float *arr = new float[arr_size];
        timer.Start();
        ParallelApplyFoo(arr, arr_size);
        timer.Stop();
        std::cout << "ParallelApplyFoo time:" << timer.GetTime() << " ms." << std::endl;
        std::cout << "arr last data:" << arr[arr_size - 1] << std::endl;
        delete[] arr;
    }

    {
        float *arr = new float[arr_size];
        timer.Start();
        ParallelApplyFooLambda(arr, arr_size);
        timer.Stop();
        std::cout << "ParallelApplyFooLambda time:" << timer.GetTime() << " ms." << std::endl;
        std::cout << "arr last data:" << arr[arr_size - 1] << std::endl;
        delete[] arr;
    }

    {
        float *arr = new float[arr_size];
        timer.Start();
        ParallelApplyFooChunk(arr, arr_size);
        timer.Stop();
        std::cout << "ParallelApplyFooChunk time:" << timer.GetTime() << " ms." << std::endl;
        std::cout << "arr last data:" << arr[arr_size - 1] << std::endl;
        delete[] arr;
    }

    {
        float *arr = new float[arr_size];
        timer.Start();
        TimeStepFoo(arr, arr_size, 1);
        timer.Stop();
        std::cout << "TimeStepFoo time:" << timer.GetTime() << " ms." << std::endl;
        std::cout << "arr last data:" << arr[arr_size - 1] << std::endl;
        delete[] arr;
    }

    return 0;
}
