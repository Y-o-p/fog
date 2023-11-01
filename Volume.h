#pragma once

#include <algorithm>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "PerlinNoise.h"
#include <array>

struct VoxelVertex {
    float value;
    glm::vec3 gradient;
};

using Voxel = std::array<VoxelVertex, 8>;
// struct Voxel {
//     VoxelVertex a, b, c, d, e, f, g, h;
// };

class Volume {
public:
    Volume(std::vector<VoxelVertex>&& data, int width, int height, int depth);
    Volume(int width, int height, int depth);
    Volume(const Volume& other);
    ~Volume();

    VoxelVertex& get_voxel(int x, int y, int z);
    void calculate_gradient();

private:
    std::vector<VoxelVertex> m_data;
    int m_width, m_height, m_depth;
};

Volume create_perlin_volume(int size);