#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <numeric>

using namespace std;

class Timer {
public:
    void start() {
        m_last = clock();
    }

    double end() {
        auto now = clock() - m_last;
        double t = (double)now / CLOCKS_PER_SEC;

        m_times.push_back(t);

        return t;
    }

    inline size_t times_length() const {
        return m_times.size();
    }

    void output_results() const {
		cout << m_times.size() << endl;
		double total_s = accumulate(m_times.begin(), m_times.end(), 0.0);
		double total_ms = total_s * 1000.0;
		double average_frame_time = total_ms / m_times.size();
		auto variance_fun = [average_frame_time](auto accumulator, auto val){
			return accumulator + pow(val - average_frame_time, 2.0);
		};
		double variance = accumulate(m_times.begin(), m_times.end(), 0.0, variance_fun) / (m_times.size() - 1);
		double average_fps = 1.0 / (average_frame_time / 1000.0);
		cout << endl;
		cout << setw(30) << setfill('.') << left << "Total rendering time" << right << total_s << "s" << endl; 
		cout << setw(30) << setfill('.') << left << "Mean frame time" << right << average_frame_time << "ms" << endl; 
		cout << setw(30) << setfill('.') << left << "Frame time variance" << right << variance << "ms" << endl; 
		cout << setw(30) << setfill('.') << left << "Mean FPS" << right << average_fps << "FPS" << endl; 
    }

private:
    clock_t m_last;
    vector<double> m_times;
};