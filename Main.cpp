// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "VolumeRenderer.h"

// Globals
#define CANVAS_WIDTH 400 
#define CANVAS_HEIGHT 400
#define CENTER_X CANVAS_WIDTH / 2
#define CENTER_Y CANVAS_HEIGHT / 2

using namespace glm;
 
auto viewing_plane = ViewingPlane<CANVAS_WIDTH, CANVAS_HEIGHT>();
auto renderer = VolumeRenderer<CANVAS_WIDTH, CANVAS_HEIGHT>();
unsigned int vao_id;
unsigned int canvas_points_id;
unsigned int shader_id;

void clear_screen() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.clear();
}

void display_func() {
	std::cout << "Starting rendering..." << std::endl;

	clear_screen();
	auto timer = Timer();
	timer.start();
	
	// Do the rendering !! 
	//renderer.draw(viewing_plane);
	glBindVertexArray(vao_id);
	glDrawArrays(GL_POINTS, 0, CANVAS_WIDTH * CANVAS_HEIGHT);

	double duration = timer.end();
	std::printf("That took %f seconds\n", duration / 1000.0);
	glFlush();

	std::cout << "Done" << std::endl;
}

std::stringstream get_file_data(const char* filename) {
	auto file = std::ifstream(filename, std::ios::out);
	std::stringstream file_data;
	file_data << file.rdbuf();
	return file_data;
}

void get_shader_errors(unsigned int shader) {
	GLint success;
	GLchar info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cerr << info_log << std::endl;
	}
}

void init_shaders() {
	// Initialize vertex shader
	std::string vertex_shader = get_file_data("shader.vert").str();
	const char* vertex_shader_data = vertex_shader.c_str();
	unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_shader_data, NULL);

	// Initalize fragment shader
	std::string fragment_shader = get_file_data("shader.frag").str();
	const char* fragment_shader_data = fragment_shader.c_str();
	//std::cout << fragment_shader_data << std::endl;
	unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_shader_data, NULL);

	// Link and compile shaders
	glCompileShader(vertex_shader_id);
	get_shader_errors(vertex_shader_id);
	glCompileShader(fragment_shader_id);
	get_shader_errors(fragment_shader_id);
	shader_id = glCreateProgram();
	glAttachShader(shader_id, vertex_shader_id);
	glAttachShader(shader_id, fragment_shader_id);
	glLinkProgram(shader_id);
	
	// Check if linking succeeded
	GLint success;
	GLchar info_log[512];
	glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_id, 512, NULL, info_log);
		std::cerr << info_log << std::endl;
	}

	glUseProgram(shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	// Uniform stuff
	int view_location = glGetUniformLocation(shader_id, "view");
	const float* view_mat = (const float*)value_ptr(viewing_plane.get_mat());
	for (int i = 0; i < 16; i++) {
		std::cout << view_mat[i] << std::endl;
	}
	glUniformMatrix4fv(view_location, 1, GL_TRUE, view_mat);

	int volume_size_location = glGetUniformLocation(shader_id, "volume_size");
	glUniform1ui(volume_size_location, CUBE_SIZE);

	int direction_location = glGetUniformLocation(shader_id, "direction_location");
	vec3 view_direction = viewing_plane.get_direction();
	glUniform3f(volume_size_location, view_direction.x, view_direction.y, view_direction.z);
}

void init_vertex_attributes() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
	glEnableVertexAttribArray(0);
}

void buffer_volume_data() {
	Volume<CUBE_SIZE, CUBE_SIZE, CUBE_SIZE> perlin = create_perlin_volume<CUBE_SIZE, CUBE_SIZE, CUBE_SIZE>();
	GLuint volume_id;
	glGenBuffers(1, &volume_id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, volume_id);
	constexpr unsigned int VOLUME_SIZE = CUBE_SIZE * CUBE_SIZE * CUBE_SIZE * sizeof(VoxelVertex);
	glBufferData(GL_SHADER_STORAGE_BUFFER, VOLUME_SIZE, perlin.get_data(), GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, volume_id);
}

void buffer_canvas_points() {
	// Create VAO
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	
	// Crate point data
	vec2 canvas_points[CANVAS_WIDTH * CANVAS_HEIGHT];
	for (int y = 0; y < CANVAS_HEIGHT; y++) {
		for (int x = 0; x < CANVAS_WIDTH; x++) {
			canvas_points[y * CANVAS_WIDTH + x] = vec2(
				(float)x / (float)CANVAS_WIDTH * 2.0f - 1.0f,
				(float)y / (float)CANVAS_HEIGHT * 2.0f - 1.0f
			);
		}
	}
	glGenBuffers(1, &canvas_points_id);
	glBindBuffer(GL_ARRAY_BUFFER, canvas_points_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(canvas_points), canvas_points, GL_STATIC_DRAW);
}

int main(int argc, char ** argv) {
	// Initial setup
	glutInit(&argc, argv);
	my_setup(CANVAS_WIDTH, CANVAS_HEIGHT, "Fog");
	
	// Set up the callbacks
	glutDisplayFunc(display_func);
	
	// Variable setup
	viewing_plane.set_orientation(glm::vec3(-128, 128, -128), glm::vec3(20, 45, 0));
	buffer_canvas_points();
	init_vertex_attributes();
	init_shaders();
	buffer_volume_data();

	// Start the main loop
	glutMainLoop();
	return 0;
}
