#pragma once

#include <algorithm>
#include <array>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "PerlinNoise.h"

#include "Util.h"

using namespace glm;

#define VOLUME_TEMPLATE template<size_t length>

constexpr size_t cube(const size_t& x) {
    return static_cast<size_t>(std::pow(x, 3));
}

union VoxelVertex {
    vec4 xyzw;
    vec3 gradient;

    inline constexpr float value() {
        return xyzw.w;
    }
};

inline constexpr size_t cartesian_to_index(ivec3 v, size_t length) {
    return v.z * length * length + v.y * length + v.x;
}

VOLUME_TEMPLATE
class Volume {
public:
    Volume(const std::array<float, cube(length)>& values) {
        // Calculate gradient
        for (int z = 0; z < length; z++) {
            for (int y = 0; y < length; y++) {
                for (int x = 0; x < length; x++) {
                    ivec3 position = {x, y, z};
                    vec3 gradient = m_calculate_gradient(values, position);
                    m_data[cartesian_to_index(position, length)] = {
                        vec4(gradient, values[cartesian_to_index(position, length)])
                    };
                }
            }
        }
    }

    inline const VoxelVertex & get(ivec3 v) const {
        return (*this)[cartesian_to_index(v, length)];
    }

    inline const VoxelVertex * get_data() const {
        return m_data.data();
    }

    const VoxelVertex & operator[](int i) const {
        return m_data[i];
    }
    //VoxelVertex operator[](ivec3 v) const {return m_data[cartesian_to_index(v, length)];}

private:    
    inline constexpr float m_safe_get(const std::array<float, cube(length)> & values, ivec3 i) {
        if (i.x < 0 || i.y < 0 || i.z < 0 ||
            i.x >= length - 1 || i.y >= length - 1 || i.z >= length - 1) {
            return 0.0f;
        }
        return values[cartesian_to_index(i, length)];
    }

    constexpr vec3 m_calculate_gradient(const std::array<float, cube(length)> & values, const ivec3& v) {
        // X-axis
        float x_before = m_safe_get(values, ivec3(v.x - 1, v.y, v.z));
        float x_after = m_safe_get(values, ivec3(v.x + 1, v.y, v.z));
        // Y-axis
        float y_before = m_safe_get(values, ivec3(v.x, v.y - 1, v.z));
        float y_after = m_safe_get(values, ivec3(v.x, v.y + 1, v.z));
        // Z-axis 
        float z_before = m_safe_get(values, ivec3(v.x, v.y, v.z - 1));
        float z_after = m_safe_get(values, ivec3(v.x, v.y, v.z + 1));
        
        vec3 gradient = {
            x_before - x_after,
            y_before - y_after,
            z_before - z_after
        };
        gradient = normalize(gradient);
        return gradient;
    }

    std::array<VoxelVertex, cube(length)> m_data;
};

VOLUME_TEMPLATE
Volume<length> create_perlin_volume() {
    const siv::PerlinNoise perlin{ 123456u };
	auto noise = std::array<float, cube(length)>();
	for (int z = 0; z < length; z++) {
		for (int y = 0; y < length; y++) {
            for (int x = 0; x < length; x++) {
                noise[cartesian_to_index({x, y, z}, length)] = {
                    (float)perlin.noise3D_01(0.05 * (double)x, 0.05 * (double)y, 0.05 * (double)z)
                };
	        }
	    }
	}
	auto volume = Volume<length>(noise);
    return volume;
}
