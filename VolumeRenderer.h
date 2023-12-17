#pragma once

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "ViewingPlane.h"
#include "Volume.h"

WIDTH_HEIGHT_DEPTH
class VolumeRenderer {
public:
VolumeRenderer() {}
VolumeRenderer(GLuint shader_id, const ViewingPlane& view) : m_shader_id(shader_id) {
	glGenVertexArrays(1, &m_vao_id);
    glGenBuffers(1, &m_volume_id);
    glGenBuffers(1, &m_canvas_points_id);

	m_direction_location = glGetUniformLocation(shader_id, "direction");
    m_view_location = glGetUniformLocation(shader_id, "view");
	m_volume_size_location = glGetUniformLocation(shader_id, "volume_size");
    glUniform1ui(m_volume_size_location, w);
    buffer_canvas_points(view);
    m_init_vertex_attributes();
}

inline void render() const {
    glBindVertexArray(m_vao_id);
	glDrawArrays(GL_POINTS, 0, m_num_vertices);
}

void buffer_canvas_points(const ViewingPlane& view) {
	glBindVertexArray(m_vao_id);
    std::vector<vec3> plane = view.get_plane(false);
    m_num_vertices = plane.size();
	glBindBuffer(GL_ARRAY_BUFFER, m_canvas_points_id);
	glBufferData(GL_ARRAY_BUFFER, m_num_vertices * sizeof(vec3), plane.data(), GL_STATIC_DRAW);
}

void set_view(const ViewingPlane& view) {
	const float* view_mat = (const float*)value_ptr(view.get_mat());
	glUniformMatrix4fv(m_view_location, 1, GL_TRUE, view_mat);

	vec3 view_direction = view.get_direction();
	glUniform3f(m_direction_location, view_direction.x, view_direction.y, view_direction.z);
}

void buffer_volume_data(const Volume<w, h, d>& volume) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_volume_id);
	constexpr unsigned int VOLUME_SIZE = w * h * d * sizeof(VoxelVertex);
	glBufferData(GL_SHADER_STORAGE_BUFFER, VOLUME_SIZE, volume.get_data(), GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_volume_id);
}

private:
void m_init_vertex_attributes() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);
}

size_t m_num_vertices;
GLuint m_vao_id;
GLuint m_volume_id;
GLuint m_direction_location;
GLuint m_view_location;
GLuint m_canvas_points_id;
GLuint m_volume_size_location;
GLuint m_shader_id;
};