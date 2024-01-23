#pragma once

#include <iostream>
#include <chrono>

using namespace std::chrono;

class Timer {
public:
    inline void start() {
        m_last = steady_clock::now();
    }

    inline double end() {
        double duration = (steady_clock::now() - m_last).count() / (double)std::nano::den;
        return duration;
    }

    void output_results(double total_s, int frames) const {
		double average_frame_time = total_s / frames;
		double average_fps = 1.0 / average_frame_time;
        std::cout << average_fps << "\n";
    }

private:
    steady_clock::time_point m_last;
};