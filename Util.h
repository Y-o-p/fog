#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <chrono>


void print(glm::vec3 v) {
    std::cout << v.x << ", " << v.y << ", " << v.z << "\n";
}

void print(glm::mat4 m) {
    for (int i = 0; i < 4; i++) {
        std::cout << m[0][i] << ", " << m[1][i] << ", " << m[2][i] << ", " << m[3][i] << "\n";
    }
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