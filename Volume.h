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

struct Voxel {
    glm::ivec3 position;
    std::array<VoxelVertex, 8> data;
};

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