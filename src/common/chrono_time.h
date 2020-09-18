#ifndef CHRONO_TIME_H
#define CHRONO_TIME_H

#include <chrono>

class ChronoTime {
public:
    ChronoTime() {}

    void Start() {
        start = std::chrono::high_resolution_clock::now();
    }

    void Stop() {
        stop = std::chrono::high_resolution_clock::now();
    }

    double GetTime() {
        return std::chrono::duration<double, std::micro>(stop - start).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point stop;
};

#endif //CHRONO_TIME_H
