#include <GL/glew.h>
#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#define GLM_SWIZZLE_XYZW

#include "OpenGL445Setup.h"
#include "Volume.h"
#include "ViewingPlane.h"
#include "Util.h"

using namespace glm;

WIDTH_HEIGHT_DEPTH
class VolumeRendererCPU {
public:
void draw(const ViewingPlane& view) {
    vec3 background_color = normalize(vec3(0, 204, 255));
    auto plane = view.get_plane();
    auto direction = view.get_direction();

    glBegin(GL_POINTS);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            auto index = y * w + x;
            auto &color = m_point_buffer[index];
            auto point = plane[index];
            float ray_value = calculate_ray(
                point,
                direction,
                500,
                1.0f
            );
            color = vec3(ray_value) + (1.0f - ray_value) * background_color;
            glColor3f(color.x, color.y, color.z);
            glVertex2i(x, y);
        }
    }
    glEnd();
}

constexpr void clear() {
    m_point_buffer.assign(m_point_buffer, vec3(0.0f));
}

private:

inline constexpr void get_barycentric_weights(vec3 pos, ivec3& b, ivec3& c, vec4& weights) {
    pos -= floor(pos);
    ivec3 a = ivec3(0);
    bvec3 cond = bvec3(pos.x > pos.y, pos.y >= pos.z, pos.z >= pos.x);
    b = ivec3(bvec3(!cond.z, !cond.x, !cond.y));
    c = ivec3(cond);
    ivec3 d = ivec3(1);
    mat3 system = inverse(mat3(a - d, b - d, c - d));
    vec3 t = system * (pos - (vec3)d);
    weights = vec4(t, 1.0 - t.x - t.y - t.z);
}

constexpr float calculate_lighting(float value, vec3 gradient, vec3 pos, vec3 viewer_pos, vec3 light_pos) {
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
    VoxelVertex voxel = VoxelVertex { vec4(0) };
    ivec3 p_floor = ivec3(floor(pos));
    if (p_floor.x < 0 || p_floor.x >= CUBE_SIZE ||
        p_floor.y < 0 || p_floor.y >= CUBE_SIZE ||
        p_floor.z < 0 || p_floor.z >= CUBE_SIZE) {
        // Do barycentric interpolation
        vec4 weights = vec4(0.0f);
        ivec3 b = ivec3(0);
        ivec3 c = ivec3(0);
        get_barycentric_weights(pos, b, c, weights);
        vec4 voxel_a = perlin.get_voxel(p_floor.x).xyzw * weights.x;
        vec4 voxel_b = perlin.get_voxel(p_floor + b).xyzw * weights.y;
        vec4 voxel_c = perlin.get_voxel(p_floor + c).xyzw * weights.z;
        vec4 voxel_d = perlin.get_voxel(p_floor + ivec3(1)).xyzw * weights.w;
        voxel = voxel_a + voxel_b + voxel_c + voxel_d;
    }
    return voxel;
}

float calculate_ray(vec3 start, vec3 dir, int steps, float step_size) {
    float value = 0.0;
    vec3 movement = dir * step_size;
    vec3 current_pos = start;
    for (float t = 0.0f; t < steps; t += step_size) {
        VoxelVertex voxel = get_nearest_voxel(current_pos);
        if (voxel.xyzw != vec4(0)) {
            value += calculate_lighting(voxel.value, voxel.gradient, current_pos, start, light_position);
        }
        current_pos += movement;
    }
    return clamp(value, 0.0, 1.0);
}

Volume<w, h, d> perlin = create_perlin_volume<w, h, d>();
std::vector<vec3> m_point_buffer;
};