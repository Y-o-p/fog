#pragma once

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "ShaderProgram.h"

VOLUME_TEMPLATE
class VolumeRenderer : public Renderer<length> {
public:
	VolumeRenderer() {}
	VolumeRenderer(const ViewingPlane& view) {
		ShaderProgram shader_program = ShaderProgram("shader.vert", "shader.frag");
		GLuint shader_id = shader_program.get_shader_id();

		glGenVertexArrays(1, &m_vao_id);
		glGenBuffers(1, &m_volume_id);
		glGenBuffers(1, &m_canvas_points_id);

		m_direction_location = glGetUniformLocation(shader_id, "direction");
		m_view_location = glGetUniformLocation(shader_id, "view");
		m_volume_size_location = glGetUniformLocation(shader_id, "volume_size");
		m_canvas_size_location = glGetUniformLocation(shader_id, "canvas_size");
		m_light_pos_location = glGetUniformLocation(shader_id, "light_position");
		m_depth_location = glGetUniformLocation(shader_id, "depth");
		m_sample_period_location = glGetUniformLocation(shader_id, "sample_period");
		buffer_canvas_points(view);
		m_init_vertex_attributes();
	}

	void render() override {
		glBindVertexArray(m_vao_id);
		glDrawArrays(GL_POINTS, 0, m_num_vertices);
	}

	void set_volume(const Volume<length> * volume) override {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_volume_id);
		constexpr unsigned int volume_size = std::pow(length, 3) * sizeof(VoxelVertex);
		glBufferData(GL_SHADER_STORAGE_BUFFER, volume_size, volume->get_data(), GL_STATIC_READ);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_volume_id);
		glUniform1ui(m_volume_size_location, (GLuint)length);
	}

	void set_view(const ViewingPlane * view) override {
		//const float* view_mat = (const float*);
		glUniformMatrix4fv(m_view_location, 1, GL_TRUE, value_ptr(view->get_mat()));

		vec3 view_direction = view->get_direction();
		glUniform3f(m_direction_location, view_direction.x, view_direction.y, view_direction.z);

		int depth = view->get_size().z;
		glUniform1i(m_depth_location, depth);

		float sample_period = view->get_sample_period();
		glUniform1f(m_sample_period_location, sample_period);
	}

	void set_light_pos(const glm::vec3 * pos) override {
		glUniform3f(m_light_pos_location, pos->x, pos->y, pos->z);
	}

	void buffer_canvas_points(const ViewingPlane& view) {
		glDisableVertexAttribArray(0);
		glBindVertexArray(m_vao_id);
		std::vector<vec3> plane = view.get_plane();
		m_num_vertices = plane.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_canvas_points_id);
		glBufferData(GL_ARRAY_BUFFER, m_num_vertices * sizeof(vec3), plane.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);

		vec2 canvas_size = view.get_size();
		glUniform2f(m_canvas_size_location, canvas_size.x, canvas_size.y);
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
	GLuint m_canvas_size_location;
	GLuint m_volume_size_location;
	GLuint m_light_pos_location;
	GLuint m_depth_location;
	GLuint m_sample_period_location;
};