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

void get_barycentric_weights(in vec3 pos, out ivec3 b, out ivec3 c, out vec4 weights) {
    pos = vec3(
        pos.x - floor(pos.x),
        pos.y - floor(pos.y),
        pos.z - floor(pos.z)
    );
    vec3 a = ivec3(0);
    bvec3 cond = greaterThanEqual(pos.xyz, pos.yzx);
    b = ivec3(not(cond.zxy));
    c = ivec3(cond);
    vec3 d = ivec3(1);
    mat3 system = inverse(mat3(a - d, b - d, c - d));
    vec3 t = (pos - d) * system;
    weights = vec4(t, 1.0 - t.x - t.y - t.z);
}

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

vec4 get_voxel(ivec3 p) {
    return data[p.z * volume_size * volume_size + p.y * volume_size + p.x];
}

vec4 get_nearest_voxel(vec3 pos) {
    vec4 voxel;
    ivec3 p_floor = ivec3(floor(pos));
    if (p_floor.x < 0 || p_floor.x >= volume_size ||
        p_floor.y < 0 || p_floor.y >= volume_size ||
        p_floor.z < 0 || p_floor.z >= volume_size) {
        voxel = vec4(0.0f);
    }
    else {
        // Do barycentric interpolation
        vec4 weights;
        ivec3 b;
        ivec3 c;
        get_barycentric_weights(pos, b, c, weights);
        vec4 voxel_a = get_voxel(p_floor) * weights.x;
        vec4 voxel_b = get_voxel(p_floor + b) * weights.y;
        vec4 voxel_c = get_voxel(p_floor + c) * weights.z;
        vec4 voxel_d = get_voxel(p_floor + ivec3(1)) * weights.w;
        voxel = voxel_a + voxel_b + voxel_c + voxel_d;
    }
    return voxel;
}

float calculate_ray(vec3 start, vec3 dir, int steps, float step_size) {
    float value = 0.0f;
    vec3 movement = dir * step_size;
    vec3 current_pos = start;
    for (float t = 0.0f; t < steps; t += step_size) {
        vec4 voxel = get_nearest_voxel(current_pos);
        value += calculate_lighting(voxel.x, voxel.yzw, current_pos, world_pos.xyz, vec3(0.0f, 200.0f, 0.0f));
        current_pos += movement;
    }
    return value;
}

void main() {
    float value = calculate_ray(world_pos.xyz, direction, 500, 1.0f);
    color = vec4(vec3(value), 1.0f);
}