// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "VolumeRenderer.h"
#include "ShaderProgram.h"
#include "OpenGL445Setup.h"

// Globals
#define CANVAS_WIDTH 800 
#define CANVAS_HEIGHT 600
#define CENTER_X CANVAS_WIDTH / 2
#define CENTER_Y CANVAS_HEIGHT / 2
#define FPS 60.0
#define UPDATE_RATE 1000.0 / FPS
#define CUBE_SIZE 128

using namespace glm;
 
ViewingPlane viewing_plane;
ShaderProgram shader_program;
VolumeRenderer<CUBE_SIZE, CUBE_SIZE, CUBE_SIZE> renderer;

float time_elapsed = 0.0f;
float rotation_x = 0.0f;
float rotation_y = 0.0f;
float rotation_z = 0.0f;

void clear_screen() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void display_func() {
	clear_screen();
	
	// Do the rendering !! 
	renderer.render();
	
	glFlush();
}

auto timer = Timer();
void update(int ID) {
	timer.start();
	time_elapsed += UPDATE_RATE / 1000.0;
	rotation_x = cos(radians(time_elapsed * 15.0)) * 10.0;
	rotation_y = sin(radians(time_elapsed * 15.0)) * 10.0;
	
	viewing_plane.set_orientation(glm::vec3(256.0f, 256.0f, -128.0f), glm::vec3(rotation_x, rotation_y, 0), glm::vec3(0.25));
	renderer.set_view(viewing_plane);

	display_func();
	glutTimerFunc(UPDATE_RATE, update, 0);
	double duration = timer.end();
	std::printf("%fms | %f FPS            \r", duration, 1.0 / (duration / 1000.0));
}

void reshape_func(int w, int h) {
	my_3d_projection(w, h);
	viewing_plane = ViewingPlane(glm::vec3(0), glm::vec3(0), glm::vec3(1), w, h);
	viewing_plane.set_orientation(glm::vec3(64, 64, -128), glm::vec3(10, 10, 0), glm::vec3(0.5));
	renderer.buffer_canvas_points(viewing_plane);
}

int main(int argc, char ** argv) {
	// Initial setup
	glutInit(&argc, argv);
	my_setup(CANVAS_WIDTH, CANVAS_HEIGHT, "Fog");
	
	// Set up the callbacks
	glutDisplayFunc(display_func);
	glutTimerFunc(UPDATE_RATE, update, 0);
	glutReshapeFunc(reshape_func);

	// Variable setup
	//viewing_plane.set_orientation(glm::vec3(-128, 128, -128), glm::vec3(20, 45, 0));
	
	shader_program = ShaderProgram("shader.vert", "shader.frag");
	renderer = VolumeRenderer<CUBE_SIZE, CUBE_SIZE, CUBE_SIZE>(shader_program.get_shader_id(), viewing_plane);
	reshape_func(CANVAS_WIDTH, CANVAS_HEIGHT);
	auto perlin = create_perlin_volume<CUBE_SIZE, CUBE_SIZE, CUBE_SIZE>();
	renderer.buffer_volume_data(perlin);

	// Start the main loop
	glutMainLoop();
	return 0;
}
