#version 430 core
layout (location = 0) in vec2 position;

uniform mat4 view;

out vec4 world_pos;

void main() {
    gl_Position = vec4(position.x, position.y, -0.1f, 1.0f);
    world_pos = vec4(200.0f * position.x, 200.0f * position.y, 0.0f, 1.0f) * view;
}