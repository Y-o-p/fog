#pragma once

#include <iostream>
#include <glm/glm.hpp>


void print(glm::vec3 v) {
    std::cout << v.x << ", " << v.y << ", " << v.z << "\n";
}