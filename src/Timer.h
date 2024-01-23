#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

using namespace std::chrono;

class Timer {
public:
    inline void start() {
        m_last = steady_clock::now();
    }

    inline double end() {
        double duration = (steady_clock::now() - m_last).count() / (double)std::nano::den;
        m_times.push_back(duration);
        return duration;
    }

    void output_results() const {
		double total_s = std::accumulate(m_times.begin(), m_times.end(), 0.0);
        double average_frame_time = total_s / 60.0;//m_times.size();
		double average_fps = 1.0 / average_frame_time;
        std::cout << average_fps << "\n";
    }

private:
    steady_clock::time_point m_last;
    std::vector<double> m_times;
};