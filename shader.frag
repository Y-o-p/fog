#version 430 core

// Ins
in vec4 world_pos;

struct VoxelVertex {
    float value;
    vec3 gradient;
};

layout(std430, binding = 0) buffer volume {
    vec4 data[];
};

uniform uint volume_size; 
uniform vec3 direction;

// Outs
out vec4 color;

float calculate_lighting(float value, vec3 gradient, vec3 pos, vec3 viewer_pos, vec3 light_pos) {
    // Ambient lighting calculation  
    const float ambient_light = 1.0f;
    const float ambient = value * ambient_light;

    // Diffuse lighting calculation
    const float diffuse_light = 1.0f;
    const vec3 light_vector = normalize(light_pos - pos);
    const float diffuse = diffuse_light * dot(light_vector, gradient);

    // Specular lighting calculation
    const float specular_light = 1.0f;
    const float shinniness = 1.0f;
    const vec3 viewer_vector = normalize(viewer_pos);
    const vec3 halfway_vector = normalize((light_vector + viewer_vector) / 2.0f);
    const float specular = specular_light * pow(dot(halfway_vector, gradient), shinniness);

    // Phong
    return clamp(ambient + diffuse + specular, 0.0f, 1.0f) / 100.0f;
}

void get_voxel(in ivec3 p, out vec4 v) {
    v = data[p.z * volume_size * volume_size + p.y * volume_size + p.x];
}

void get_nearest_voxel(in vec3 p, out vec4 v) {
    ivec3 p_floor = ivec3(floor(p));
    if (p_floor.x < 0 || p_floor.x >= volume_size ||
        p_floor.y < 0 || p_floor.y >= volume_size ||
        p_floor.z < 0 || p_floor.z >= volume_size) {
        v = vec4(0.0f);
    }
    else {
        get_voxel(p_floor, v);
    }
}

void calculate_ray(in vec3 start, in vec3 dir, in int steps, in float step_size, out float value) {
    value = 0.0f;
    vec3 movement = dir * step_size;
    vec3 current_pos = start;
    for (float t = 0.0f; t < steps; t += step_size) {
        vec4 voxel;
        get_nearest_voxel(current_pos, voxel);
        value += calculate_lighting(voxel.x, voxel.yzw, current_pos, world_pos.xyz, vec3(0.0f, 200.0f, 0.0f));
        current_pos += movement;
    }
}

void main() {
    float value;
    calculate_ray(world_pos.xyz, direction, 500, 1.0f, value);
    color = vec4(vec3(value), 1.0f);
}