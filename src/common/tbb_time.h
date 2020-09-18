#ifndef TBB_TIME_H
#define TBB_TIME_H

#include "tbb/tick_count.h"

class TbbTime {
public:
    TbbTime() {}

    void Start() {
        start = tbb::tick_count::now();
    }

    void Stop() {
        stop = tbb::tick_count::now();
    }

    double GetTime() {
        return (stop - start).seconds() * 1000;   //ms
    }

private:
    tbb::tick_count start;
    tbb::tick_count stop;
};

#endif //TBB_TIME_H
