#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 view;
uniform vec2 canvas_size;

out vec4 world_pos;

void main() {
    gl_Position = vec4(position.x, position.y, -0.1f, 1.0f);
    world_pos = vec4(position, 1.0f) * view;
}
