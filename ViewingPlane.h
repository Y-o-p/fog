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
    ViewingPlane(vec3 position = vec3(0), vec3 rotation = vec3(0), int width = 400, int height = 400): 
    m_width(width), m_height(height), m_mat(mat4(1)) {
        m_data.resize(m_width * m_height);
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                m_data[y * m_width + x] = vec3(
                    (float)x / (float)m_height * 2.0f - 1.0f,
                    (float)y / (float)m_height * 2.0f - 1.0f,
                    0
                );
            }
        }
        std::cout << "Constructed with " << m_data.size() << std::endl;
        set_orientation(position, rotation);
    }

    void set_orientation(vec3 position, vec3 rotation) {
        m_mat = mat4(1);
        m_mat = translate(m_mat, position);
        m_mat = rotate(m_mat, radians(rotation.z), vec3(0, 0, 1));
        m_mat = rotate(m_mat, radians(rotation.y), vec3(0, 1, 0));
        m_mat = rotate(m_mat, radians(rotation.x), vec3(1, 0, 0));
    }

    std::vector<vec3> get_plane(bool transformed = true) const {
        auto oriented_data = m_data;
        if (transformed) {
            for (auto& point : oriented_data) {
                point = m_mat * vec4(point, 1.0f);
            }
        }
        return oriented_data;
    }

    mat4 get_mat() const {
        return m_mat;
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