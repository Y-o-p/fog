#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

inline void print(glm::vec3 v) {
    std::cout << v.x << ", " << v.y << ", " << v.z << "\n";
}

inline void print(glm::mat4 m) {
    for (int i = 0; i < 4; i++) {
        std::cout << m[0][i] << ", " << m[1][i] << ", " << m[2][i] << ", " << m[3][i] << "\n";
    }
}

inline std::stringstream get_file_data(const char* filename) {
	auto file = std::ifstream(filename, std::ios::out);
	if (file.is_open()) {
        std::stringstream file_data;
	    file_data << file.rdbuf();
	    return file_data;
    }
    else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
    return std::stringstream();
}

class Timer {
public:
    void start() {
        m_last = std::chrono::high_resolution_clock::now();
    }

    double end() {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> dur = now - m_last;
        return dur.count();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_last;
};