// #include <GL/glew.h>
// #include <iostream>
// #include <GL/freeglut.h>
// #include <cmath>

// #include "OpenGL445Setup.h"
// #include "Volume.h"
// #include "ViewingPlane.h"
// #include "Util.h"

// using namespace glm;
// #define CUBE_SIZE 128

// WIDTH_HEIGHT
// class VolumeRendererCPU {
// public:
// //void render(const Volume& object);
// void draw(const ViewingPlane<w, h>& view) {
//     vec3 background_color = normalize(vec3(0, 204, 255));
//     auto plane = view.get_plane();
//     auto direction = view.get_direction();

//     glBegin(GL_POINTS);
//     for (int y = 0; y < h; y++) {
//         for (int x = 0; x < w; x++) {
//             auto index = y * w + x;
//             auto &color = m_point_buffer[index];
//             auto point = plane[index];
//             float ray_value = calculate_ray(
//                 point,
//                 direction,
//                 500,
//                 1.0f
//             );
//             color = vec3(ray_value) + (1.0f - ray_value) * background_color;
//             glColor3f(color.x, color.y, color.z);
//             glVertex2i(x, y);
//         }
//     }
//     glEnd();
// }

// constexpr void clear() {
//     for (int i = 0; i < w * h; i++) {
//         m_point_buffer[i] = vec3(0.0f);
//     }
// }

// private:
// constexpr u8 get_tetrahedron(vec3 p) {
//     u8 outcome = 0;
//     outcome |= 0b100 & ((p.x > p.y) << 2);
//     outcome |= 0b010 & ((p.z > p.y) << 1);
//     outcome |= 0b001 & (p.x > p.z);
//     return outcome;
// }

// vec4 get_bary_coords(const vec3& p, const vec3& a, const vec3& b, const vec3& c, const vec3& d) {
//     mat3 m = mat3 {
//         a.x - d.x, a.y - d.y, a.z - d.z,
//         b.x - d.x, b.y - d.y, b.z - d.z,
//         c.x - d.x, c.y - d.y, c.z - d.z,
//     };

//     vec3 t = glm::inverse(m) * (p - d);
//     vec4 baries = vec4(t, 1.0f - t.x - t.y - t.z);

//     return baries;
// }

// constexpr VoxelVertex get_voxel_vertex(const ivec3& p) {
//     if (p.x < 0 || p.x > CUBE_SIZE - 1 ||
//         p.y < 0 || p.y > CUBE_SIZE - 1 ||
//         p.z < 0 || p.z > CUBE_SIZE - 1) {
//         return VoxelVertex {
//             0.0f,
//             vec3(0.0f),
//         };
//     }

//     return perlin.get_voxel(p.x, p.y, p.z);
// }

// Voxel get_nearest_voxel(vec3 p) {
//     ivec3 p_floor = floor(p);
//     ivec3 points[8] = {
//         p_floor + VERTICES[0],
//         p_floor + VERTICES[1],
//         p_floor + VERTICES[2],
//         p_floor + VERTICES[3],
//         p_floor + VERTICES[4],
//         p_floor + VERTICES[5],
//         p_floor + VERTICES[6],
//         p_floor + VERTICES[7],
//     };

//     Voxel nearest_voxel;
//     for (int i = 0; i < nearest_voxel.data.size(); i++) {
//         nearest_voxel.data[i] = get_voxel_vertex(points[i]);
//     }
//     nearest_voxel.position = std::move(points[0]);
//     return nearest_voxel;
// }

// float calculate_lighting(float value, vec3 gradient, vec3 pos, vec3 viewer_pos, vec3 light_pos) {
//     // Ambient lighting calculation  
//     const float ambient_light = 1.0f;
//     const float ambient = value * ambient_light;

//     // Diffuse lighting calculation
//     const float diffuse_light = 1.0f;
//     const vec3 light_vector = glm::normalize(light_pos - pos);
//     const float diffuse = diffuse_light * glm::dot(light_vector, gradient);

//     // Specular lighting calculation
//     const float specular_light = 1.0f;
//     const float shinniness = 1.0f;
//     const vec3 viewer_vector = glm::normalize(viewer_pos);
//     const vec3 halfway_vector = glm::normalize((light_vector + viewer_vector) / 2.0f);
//     const float specular = specular_light * glm::pow(glm::dot(halfway_vector, gradient), shinniness);

//     // Phong
//     return clamp(ambient + diffuse + specular, 0.0f, 1.0f) / 100.0f;
// }

// float calculate_ray(vec3 start, vec3 dir, int steps = 100, float step = 0.5f) {
//     dir = normalize(dir);
//     float color = 0.0f;
    
//     for (float t = 0; t < steps; t += step) {
//         // Get the voxel that we're inside of
//         vec3 current_pos = start + t * dir;
//         Voxel vox = get_nearest_voxel(current_pos);
        
//         // Get the tetrahedron that we're inside of
//         vec3 normalized_pos = current_pos - (vec3)vox.position;
//         u8 tetra = get_tetrahedron(normalized_pos);
//         u8 a = 0;
//         u8 b = 0;
//         u8 c = 0;
//         u8 d = 0;
//         if (tetra == TETRA::A) {
//             b = 1;
//             c = 5;
//             d = 6;
//             //std::cout << "A ";
//         }
//         else if (tetra == TETRA::B) {
//             b = 1;
//             c = 2;
//             d = 6;
//             //std::cout << "B ";
//         }
//         else if (tetra == TETRA::C) {
//             b = 2;
//             c = 3;
//             d = 6;
//             //std::cout << "C ";
//         }
//         else if (tetra == TETRA::D) {
//             b = 3;
//             c = 6;
//             d = 7;
//             //std::cout << "D ";
//         }
//         else if (tetra == TETRA::E) {
//             b = 4;
//             c = 6;
//             d = 7;
//             //std::cout << "E ";
//         }
//         else if (tetra == TETRA::F) {
//             b = 4;
//             c = 5;
//             d = 6;
//             //std::cout << "F ";
//         }
        
//         // Do barycentric interpolation
//         vec4 baries = get_bary_coords(normalized_pos, VERTICES[a], VERTICES[b], VERTICES[c], VERTICES[d]);
//         //std::cout << baries.x << ", " << baries.y << ", " << baries.z << ", " << baries.w << std::endl;
    
//         // Interpolate vertex attributes
//         float value = vox.data[a].value * baries.x +
//                     vox.data[b].value * baries.y +
//                     vox.data[c].value * baries.z +
//                     vox.data[d].value * baries.w;
//         vec3 gradient = vox.data[a].gradient * baries.x +
//                         vox.data[b].gradient * baries.y +
//                         vox.data[c].gradient * baries.z +
//                         vox.data[d].gradient * baries.w;
        
//         // Lighting
//         if (gradient != vec3(0)) {
//             color += calculate_lighting(value, gradient, current_pos, start, glm::vec3(0.0f, 0.0f, 0.0f));
//             if (color >= 1.0f) {
//                 color = 1.0f;
//                 break;
//             }
//         }
//     }

//     return color;
// }

// Volume<CUBE_SIZE, CUBE_SIZE, CUBE_SIZE> perlin = create_perlin_volume<CUBE_SIZE, CUBE_SIZE, CUBE_SIZE>();
// vec3 m_point_buffer[w * h];
// enum TETRA {
//     A = 0b111,
//     B = 0b101,
//     C = 0b001,
//     D = 0b000,
//     E = 0b010,
//     F = 0b110,
// };
// const ivec3 VERTICES[8] = {
//     {0, 0, 0},
//     {1, 0, 0},
//     {1, 1, 0},
//     {0, 1, 0},
//     {0, 0, 1},
//     {1, 0, 1},
//     {1, 1, 1},
//     {0, 1, 1},
// };
// };