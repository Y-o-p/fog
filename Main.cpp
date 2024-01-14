// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>


// Globals
#define CANVAS_WIDTH 400 
#define CANVAS_HEIGHT 400
#define CENTER_X CANVAS_WIDTH / 2
#define CENTER_Y CANVAS_HEIGHT / 2
#define FPS 60.0
#define UPDATE_RATE 1000.0 / FPS
#define CUBE_SIZE 63
#define CAMERA_SCALE 0.25

#define CPU_BASED

using namespace glm;
 
#ifdef CPU_BASED
#include "VolumeRendererCPU.h"
Volume<CUBE_SIZE> perlin = create_perlin_volume<CUBE_SIZE>();
VolumeRendererCPU<CUBE_SIZE> renderer = VolumeRendererCPU<CUBE_SIZE>();
#else
#include "VolumeRenderer.h"
VolumeRenderer<CUBE_SIZE> renderer;
#endif

ViewingPlane viewing_plane = ViewingPlane(CANVAS_WIDTH, CANVAS_HEIGHT);
#include "OpenGL445Setup.h"

float time_elapsed = 0.0f;
float rotation_x = 0.0f;
float rotation_y = 0.0f;
float rotation_z = 0.0f;
vec3 light_pos = vec3(0);

void clear_screen() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void display_func() {
	clear_screen();
	light_pos = glm::vec3(
		cos(radians(time_elapsed * 100.0)) * 100.0 + 32,
		64,
		sin(radians(time_elapsed * 100.0)) * 100.0 + 32
	);
	viewing_plane.set_orientation(
		glm::vec3(-128, 120, -128),
		glm::vec3(20, 45, 0),
		glm::vec3(CANVAS_WIDTH / 2.0f * CAMERA_SCALE, CANVAS_HEIGHT / 2.0f * CAMERA_SCALE, 1.0f)
	);
	renderer.set_light_pos(&light_pos);
	renderer.set_view(&viewing_plane);
	renderer.render();
	glFlush();
}

auto timer = Timer();
void update(int ID) {
	timer.start();
	time_elapsed += UPDATE_RATE / 1000.0;
	
	display_func();

	glutTimerFunc(UPDATE_RATE, update, 0);
	double duration = timer.end();
	std::printf("%fms | %f FPS            \r", duration, 1.0 / (duration / 1000.0));
	std::fflush(stdout);
}

int main(int argc, char ** argv) {
	// Initial setup
	glutInit(&argc, argv);
	my_setup(CANVAS_WIDTH, CANVAS_HEIGHT, "Fog");
	
	// Set up the callbacks
	glutDisplayFunc(display_func);
	glutTimerFunc(UPDATE_RATE, update, 0);

#ifndef CPU_BASED
	auto perlin = create_perlin_volume<CUBE_SIZE>();
	renderer = VolumeRenderer<CUBE_SIZE>(viewing_plane);
#endif
	renderer.set_volume(&perlin);

	// Start the main loop
	glutMainLoop();
	return 0;
}
