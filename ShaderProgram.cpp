#include "ShaderProgram.h"

void ShaderProgram::initialize(const char *vertex_shader, const char *fragment_shader) {
    m_vertex_shader = get_file_data(vertex_shader).str();
	m_fragment_shader = get_file_data(fragment_shader).str();
    
    // Initialize vertex shader
	const char* vertex_shader_data = m_vertex_shader.c_str();
	unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_shader_data, NULL);

	// Initalize fragment shader
	const char* fragment_shader_data = m_fragment_shader.c_str();
	unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_shader_data, NULL);

	// Link and compile shaders
	glCompileShader(vertex_shader_id);
	m_print_shader_errors(vertex_shader_id);
	glCompileShader(fragment_shader_id);
	m_print_shader_errors(fragment_shader_id);
	m_shader_id = glCreateProgram();
	glAttachShader(m_shader_id, vertex_shader_id);
	glAttachShader(m_shader_id, fragment_shader_id);
	glLinkProgram(m_shader_id);
	
	// Check if linking succeeded
	GLint success;
	GLchar info_log[512];
	glGetProgramiv(m_shader_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shader_id, 512, NULL, info_log);
		std::cerr << info_log << std::endl;
	}

	glUseProgram(m_shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
}

void ShaderProgram::m_print_shader_errors(unsigned int shader) {
	GLint success;
	GLchar info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cerr << info_log << std::endl;
	}
}

