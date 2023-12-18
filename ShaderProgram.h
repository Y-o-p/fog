#pragma once

#include <GL/glew.h>
#include <iostream>
#include <GL/freeglut.h>
#include <cmath>

#include "Util.h"

using namespace glm;

class ShaderProgram {
public:
ShaderProgram() {}
ShaderProgram(const char* vertex_shader, const char* fragment_shader) {
	initialize(vertex_shader, fragment_shader);
    
}

void initialize(const char* vertex_shader, const char* fragment_shader);
constexpr GLuint get_shader_id() const {
    return m_shader_id;
}

private:
void m_print_shader_errors(unsigned int shader);

// Member variables
std::string m_vertex_shader;
std::string m_fragment_shader;
GLuint m_shader_id;
};