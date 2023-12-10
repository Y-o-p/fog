#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Util.h"

#define WIDTH_HEIGHT template<std::size_t w, std::size_t h>

WIDTH_HEIGHT
class ViewingPlane {
public:
    constexpr ViewingPlane(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0)) {
        m_data.resize(w*h);
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                m_data[y * w + x] = glm::vec3(x - (float)w / 2, y - (float)h / 2, 0);
            }
        }
        // std::cout << data[0].x << ", " << data[0].y << ", " << data[0].z << std::endl;
        // std::cout << data[1].x << ", " << data[1].y << ", " << data[1].z << std::endl;
        // std::cout << data[w].x << ", " << data[w].y << ", " << data[w].z << std::endl;
        std::cout << "Constructed with " << m_data.size() << std::endl;
        set_orientation(position, rotation);
    }

    constexpr void set_orientation(glm::vec3 position, glm::vec3 rotation) {
        m_mat = glm::mat4(1);
        m_mat = glm::translate(m_mat, position);
        m_mat = glm::rotate(m_mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        m_mat = glm::rotate(m_mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        m_mat = glm::rotate(m_mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        //std::cout << mat[0][0] << ", " << mat[2][2] << std::endl;
        //mat = glm::translate(mat, position);
    }

    constexpr std::vector<glm::vec3> get_plane() const {
        auto oriented_data = m_data;
        for (auto& point : oriented_data) {
            point = m_mat * glm::vec4(point, 1.0f);
        }
        return oriented_data;
    }

    constexpr glm::vec3 get_direction() const {
        auto a = glm::vec3(m_mat * glm::vec4(m_data[0], 1.0f)); // Origin
        auto b = glm::vec3(m_mat * glm::vec4(m_data[w - 1], 1.0f)); // Rightmost point
        auto c = glm::vec3(m_mat * glm::vec4(m_data[w * (h - 1)], 1.0f)); // Upmost point
        return glm::cross(glm::vec3(b - a), glm::vec3(c - a));
    }

private:
    std::vector<glm::vec3> m_data;
    glm::mat4 m_mat;
};