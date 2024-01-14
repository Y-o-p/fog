#include <GL/glew.h>
#include <iostream>
#include <GL/freeglut.h>
#include <cmath>

#include "Renderer.h"

using namespace glm;

VOLUME_TEMPLATE
class VolumeRendererCPU : public Renderer<length> {
public:
    void render() override {
        vec2 canvas_size = m_view->get_size();
        const std::vector<vec3>& plane = m_view->get_plane();
        vec3 direction = m_view->get_direction();
        mat4 view_mat = m_view->get_mat();

        glBegin(GL_POINTS);
        for (int y = 0; y < canvas_size.x; y++) {
            for (int x = 0; x < canvas_size.x; x++) {
                auto index = y * canvas_size.x + x;
                vec4 point = view_mat * vec4(plane[index], 1.0f);
                float ray_value = m_calculate_ray(
                    point,
                    direction,
                    500,
                    1.0f,
                    *m_light_pos
                );
                vec3 color = vec3(ray_value);
                glColor3f(color.x, color.y, color.z);
                glVertex2i(x, y);
            }
        }
        glEnd();
    }

    void set_volume(const Volume<length> * volume) override {
        m_volume = volume;
    }

    void set_view(const ViewingPlane * view) override {
        m_view = view;
    }

    void set_light_pos(const vec3 * pos) override {
        m_light_pos = pos;
    }

private:
    constexpr float m_calculate_lighting(float value, vec3 gradient, vec3 pos, vec3 viewer_pos, vec3 light_pos) {
        // Ambient lighting calculation  
        const float ambient_light = 1.0;
        const float ambient = value * ambient_light;

        // Diffuse lighting calculation
        const float diffuse_light = 1.5;
        const vec3 light_vector = normalize(light_pos - pos);
        float diffuse = diffuse_light * dot(light_vector, gradient);
        diffuse = max(0.0f, diffuse);

        // Specular lighting calculation
        const float specular_light = 2.0;
        const float shinniness = 5.0;
        const vec3 viewer_vector = normalize(viewer_pos - pos);
        const vec3 halfway_vector = normalize((light_vector + viewer_vector) / 2.0f);
        float specular = specular_light * pow(dot(halfway_vector, gradient), shinniness);
        specular = max(0.0f, specular);

        // Phong
        float final = (ambient + diffuse + specular) / 100.0;
        return final;
    }

    constexpr VoxelVertex get_nearest_voxel(vec3 pos) {
        VoxelVertex voxel = {vec4(0)};
        ivec3 p_floor = ivec3(floor(pos));
        if (!(p_floor.x < 0 || p_floor.x >= length - 1 ||
            p_floor.y < 0 || p_floor.y >= length - 1 ||
            p_floor.z < 0 || p_floor.z >= length - 1)) {
            // Do barycentric interpolation
            vec4 weights = vec4(0.0f);
            ivec3 b = ivec3(0);
            ivec3 c = ivec3(0);
            get_barycentric_weights(pos, b, c, weights);
            vec4 voxel_a = m_volume->get(p_floor).xyzw    * weights.x;
            vec4 voxel_b = m_volume->get(p_floor + b).xyzw * weights.y;
            vec4 voxel_c = m_volume->get(p_floor + c).xyzw * weights.z;
            vec4 voxel_d = m_volume->get(p_floor + ivec3(1)).xyzw * weights.w;
            voxel = {voxel_a + voxel_b + voxel_c + voxel_d};
        }
        return voxel;
    }

    constexpr float m_calculate_ray(vec3 start, vec3 dir, int steps, float step_size, vec3 light_position = {0, 0, 0}) {
        float value = 0.0;
        vec3 movement = dir * step_size;
        vec3 current_pos = start;
        for (float t = 0.0f; t < steps; t += step_size) {
            VoxelVertex voxel = get_nearest_voxel(current_pos);
            if (voxel.gradient != vec3(0.0f)) {
                value += calculate_lighting(voxel.value(), voxel.gradient, current_pos, start, light_position);
            }
            current_pos += movement;
        }
        return clamp(value, 0.0f, 1.0f);
    }

    const Volume<length> * m_volume;
    const ViewingPlane * m_view;
    const vec3 * m_light_pos;
};