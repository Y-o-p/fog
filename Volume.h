#pragma once

#include <algorithm>
#include <vector>
#include <glm/vec3.hpp>
#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include "PerlinNoise.h"
#include <iostream>
#include <array>
#include "Util.h"

#define WIDTH_HEIGHT_DEPTH template<std::size_t w, std::size_t h, std::size_t d>

union VoxelVertex {
    glm::vec4 xyzw;
    struct {
        float value;
        glm::vec3 gradient;
    };
};

struct Voxel {
    glm::ivec3 position;
    std::array<VoxelVertex, 8> data;
};

WIDTH_HEIGHT_DEPTH
class Volume {
public:
    constexpr Volume(std::vector<VoxelVertex>&& data) : 
    m_width(w), m_height(h), m_depth(d) {
        m_data = data;
        calculate_gradient();
    }

    constexpr Volume() :
    m_width(w), m_height(h), m_depth(d) {
        m_data = std::vector<VoxelVertex>();
    }

    constexpr Volume(const Volume& other)  {
        m_width = other.m_width;
        m_height = other.m_height;
        m_depth = other.m_depth;
        m_data = other.m_data;
    }

    constexpr VoxelVertex& get_voxel(ivec3 i) {
        return m_data[i.z * w * h + i.y * h + i.x];
    }

    const VoxelVertex *const get_data() const {
        return m_data.data();
    } 

    constexpr void calculate_gradient() {
        for (int z = 1; z < m_depth - 1; z++) {
            for (int y = 1; y < m_height - 1; y++) {
                for (int x = 1; x < m_width - 1; x++) {
                    VoxelVertex& v = get_voxel(x, y, z);
                    
                    // X-axis gradient calculation
                    VoxelVertex& x_before = get_voxel(x - 1, y, z);
                    VoxelVertex& x_after = get_voxel(x + 1, y, z);
                    v.gradient.x = (x_before.value - x_after.value);

                    // Y-axis gradient calculation
                    VoxelVertex& y_before = get_voxel(x, y - 1, z);
                    VoxelVertex& y_after = get_voxel(x, y + 1, z);
                    v.gradient.y = (y_before.value - y_after.value);

                    // Z-axis gradient calculation
                    VoxelVertex& z_before = get_voxel(x, y, z - 1);
                    VoxelVertex& z_after = get_voxel(x, y, z + 1);
                    v.gradient.z = (z_before.value - z_after.value);

                    v.gradient = glm::normalize(v.gradient);
                }
            }
        }
    }
private:
    std::vector<VoxelVertex> m_data;
    std::size_t m_width, m_height, m_depth;
};

WIDTH_HEIGHT_DEPTH
constexpr Volume<w, h, d> create_perlin_volume() {
    const siv::PerlinNoise perlin{ 123456u };
	auto noise = std::vector<VoxelVertex>(w*h*d);
	for (int z = 0; z < d; z++) {
		for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                noise[z * w*h + y*h + x] = {
                    (float)perlin.noise3D_01(0.05 * (double)x, 0.05 * (double)y, 0.05 * (double)z),
                    glm::vec3(0.0f)
                };
	        }
	    }
	}
	auto volume = Volume<w, h, d>(std::move(noise));
    return volume;
}
