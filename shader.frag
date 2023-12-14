#version 430 core

// Ins
in vec4 world_pos;

struct VoxelVertex {
    float value;
    vec3 gradient;
};

layout(std430, binding = 0) buffer volume {
    VoxelVertex data[];
};

uniform uint volume_size; 
uniform vec3 direction;

// Outs
out vec4 color;

void get_nearest_voxel(in vec3 p, out VoxelVertex v) {
    ivec3 p_floor = ivec3(floor(p));
    if (p_floor.x < 0 || p_floor.x >= volume_size ||
        p_floor.y < 0 || p_floor.y >= volume_size ||
        p_floor.z < 0 || p_floor.z >= volume_size) {
        v = VoxelVertex(0.0f, vec3(0.0f));
    }
    v = data[p_floor.z * volume_size * volume_size + p_floor.y * volume_size + p_floor.x];
}

void calculate_ray(in vec3 start, in vec3 dir, in int steps, in float step_size, out float value) {
    value = 0.0f;
    vec3 movement = step_size * dir;
    vec3 current_pos = start;
    for (float t = 0.0f; t < steps; t += step_size) {
        VoxelVertex voxel;
        get_nearest_voxel(current_pos, voxel);
        value += voxel.value;
        current_pos += movement;
    }

    value /= 500.0f;
}

void main() {
    float value;
    calculate_ray(world_pos.xyz, direction, 500, 1.0f, value);
    color = vec4(vec3(value), 1.0f);
}