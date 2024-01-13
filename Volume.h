#pragma once

#include <algorithm>
#include <array>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "PerlinNoise.h"
#include <iostream>
#include <array>
#include "Util.h"
#include <cmath>

using namespace glm;

#define VOLUME_TEMPLATE template<size_t length>

constexpr size_t cube(const size_t& x) {
    return static_cast<size_t>(std::pow(x, 3));
}

struct VoxelVertex {
    float value;
    vec3 gradient;

    VoxelVertex operator+(const VoxelVertex& other) const {
        return VoxelVertex {
            value + other.value,
            gradient + other.gradient
        };
    }

    VoxelVertex operator*(float s) const {
        return VoxelVertex {
            value * s,
            gradient * s
        };
    }
};

VOLUME_TEMPLATE
class Volume {
public:
    constexpr Volume(std::array<VoxelVertex, cube(length)>&& data) {
        m_data = data;
        calculate_gradient();
    }

    constexpr Volume() {
        m_data = std::array<VoxelVertex, cube(length)>();
    }

    constexpr Volume(const Volume<length>& other) {
        m_data = other.m_data;
    }

    constexpr VoxelVertex& get_voxel(ivec3 i) {
        return m_data[i.z * length * length + i.y * length + i.x];
    }

    constexpr VoxelVertex get_voxel(ivec3 i) const {
        return m_data[i.z * length * length + i.y * length + i.x];
    }

    inline const VoxelVertex *const get_data() const {
        return m_data.data();
    }

    constexpr void calculate_gradient() {
        for (int z = 1; z < length - 1; z++) {
            for (int y = 1; y < length - 1; y++) {
                for (int x = 1; x < length - 1; x++) {
                    VoxelVertex& v = get_voxel(ivec3(x, y, z));
                    
                    // X-axis gradient calculation
                    VoxelVertex& x_before = get_voxel(ivec3(x - 1, y, z));
                    VoxelVertex& x_after = get_voxel(ivec3(x + 1, y, z));
                    v.gradient.x = (x_before.value - x_after.value);

                    // Y-axis gradient calculation
                    VoxelVertex& y_before = get_voxel(ivec3(x, y - 1, z));
                    VoxelVertex& y_after = get_voxel(ivec3(x, y + 1, z));
                    v.gradient.y = (y_before.value - y_after.value);

                    // Z-axis gradient calculation
                    VoxelVertex& z_before = get_voxel(ivec3(x, y, z - 1));
                    VoxelVertex& z_after = get_voxel(ivec3(x, y, z + 1));
                    v.gradient.z = (z_before.value - z_after.value);

                    v.gradient = normalize(v.gradient);
                }
            }
        }
    }
private:
    std::array<VoxelVertex, cube(length)> m_data;
};

VOLUME_TEMPLATE
constexpr Volume<length> create_perlin_volume() {
    const siv::PerlinNoise perlin{ 123456u };
	auto noise = std::array<VoxelVertex, cube(length)>();
	for (int z = 0; z < length; z++) {
		for (int y = 0; y < length; y++) {
            for (int x = 0; x < length; x++) {
                noise[z * length * length + y * length + x] = {
                    (float)perlin.noise3D_01(0.05 * (double)x, 0.05 * (double)y, 0.05 * (double)z),
                    vec3(0.0f)
                };
	        }
	    }
	}
	auto volume = Volume<length>(std::move(noise));
    return volume;
}
