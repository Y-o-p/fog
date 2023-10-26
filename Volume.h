#pragma once

#include <algorithm>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

struct Voxel {
    float value;
    glm::vec3 gradient;
};

class Volume {
public:
    Volume(std::vector<Voxel>&& data, int width, int height, int depth);
    Volume(int width, int height, int depth);
    Volume(const Volume& other);
    ~Volume();

    Voxel& get_voxel(int x, int y, int z);
    void calculate_gradient();

    std::vector<float> get_raw_values() const;

private:
    std::vector<Voxel> m_data;
    int m_width, m_height, m_depth;
};