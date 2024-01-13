#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace glm;
// TODO put these definitions in CPP
class ViewingPlane {
public:
    ViewingPlane(int width = 400, int height = 400, vec3 position = vec3(0), vec3 rotation = vec3(0), vec3 scaling = vec3(1)): 
    m_width(width), m_height(height), m_mat(mat4(1)) {
        m_data.resize(m_width * m_height);
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                m_data[y * m_width + x] = vec3(
                    (float)x / (float)m_width * 2.0f - 1.0f,
                    (float)y / (float)m_height * 2.0f - 1.0f,
                    0
                );
            }
        }
        std::cout << "Constructed ViewingPlane with " << m_data.size() << std::endl;
        set_orientation(position, rotation, scaling);
    }

    void set_orientation(vec3 position, vec3 rotation, vec3 scaling) {
        // Translation
        mat4 translation_mat = translate(mat4(1.0), position);
        // Rotations
        mat4 rot_x = rotate(mat4(1.0), radians(rotation.x), vec3(1, 0, 0));
        mat4 rot_y = rotate(mat4(1.0), radians(rotation.y), vec3(0, 1, 0));
        mat4 rot_z = rotate(mat4(1.0), radians(rotation.z), vec3(0, 0, 1));
        mat4 rotation_mat = rot_z * rot_y * rot_x;
        // Scale
        mat4 scaling_mat = glm::scale(mat4(1.0), scaling);
        // Final
        m_mat = translation_mat * rotation_mat * scaling_mat;
    }

    const std::vector<vec3>& get_plane() const {
        return m_data;
    }

    mat4 get_mat() const {
        return m_mat;
    }

    vec2 get_size() const {
        return vec2(m_width, m_height);
    }

    vec3 get_direction() const {
        auto a = vec3(m_mat * vec4(m_data[0], 1.0f)); // Origin
        auto b = vec3(m_mat * vec4(m_data[m_width - 1], 1.0f)); // Rightmost point
        auto c = vec3(m_mat * vec4(m_data[m_width * (m_height - 1)], 1.0f)); // Upmost point
        return normalize(cross(vec3(b - a), vec3(c - a)));
    }

private:
    int m_width, m_height;
    std::vector<vec3> m_data;
    mat4 m_mat;
};