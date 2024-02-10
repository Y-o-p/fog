// g++ *.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>
#include <chrono>


// Globals
static constexpr auto CANVAS_WIDTH = 400;
static constexpr auto CANVAS_HEIGHT = 400;
static constexpr auto CENTER_X = CANVAS_WIDTH / 2;
static constexpr auto CENTER_Y = CANVAS_HEIGHT / 2;
static constexpr auto FPS = 60.0;
static constexpr auto UPDATE_RATE = 1000.0 / FPS;
static constexpr auto CUBE_SIZE = 60;
static constexpr auto CAMERA_SCALE = 0.25;
static constexpr auto BACK_PLANE = 128;
static constexpr auto SAMPLE_PERIOD = 2.0f;

using namespace glm;
 
#ifdef CPU_BASED
#include "VolumeRendererCPU.h"
static Volume<CUBE_SIZE> perlin = create_perlin_volume<CUBE_SIZE>();
static VolumeRendererCPU<CUBE_SIZE> renderer = VolumeRendererCPU<CUBE_SIZE>();
#else
#include "VolumeRenderer.h"
static VolumeRenderer<CUBE_SIZE> renderer;
#endif

static ViewingPlane viewing_plane = ViewingPlane(CANVAS_WIDTH, CANVAS_HEIGHT, BACK_PLANE, SAMPLE_PERIOD);
#include "OpenGL445Setup.h"
#include "Timer.h"
#include <thread>

static float time_elapsed = 0.0f;
static float rotation_x = 0.0f;
static float rotation_y = 0.0f;
static float rotation_z = 0.0f;
static vec3 light_pos = vec3(0);
static int frame_number = 0;
static Timer timer;

void clear_screen() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void display_func() {
	clear_screen();
	light_pos = glm::vec3(
		cos(radians(time_elapsed * 100.0)) * 100.0 + 32,
		64,
		sin(radians(time_elapsed * 100.0)) * 100.0 + 32
	);
	
	renderer.set_light_pos(&light_pos);
	renderer.set_view(&viewing_plane);
	renderer.render();
	glFinish();
}

void update(int ID) {	
	if (frame_number++ < 60) {
		timer.start();
		display_func();
		timer.end();
		glutTimerFunc(UPDATE_RATE, update, 0);
		time_elapsed += UPDATE_RATE / 1000.0;
	}
	else {
		timer.output_results();
		exit(0);
	}
}

// void performance_test() {
// 	display_func();
// 	display_func();
// 	glutPostRedisplay();
// 	auto timer = Timer();
// 	timer.start();
// 	for (int i = 0; i < 60; i++) {
// 		display_func();
// 	}
// 	double duration = timer.end();
// 	std::cout << "Duration: " << duration << std::endl;
// 	timer.output_results();
// }

int main(int argc, char ** argv) {
	std::cout << std::thread::hardware_concurrency() << std::endl;
	if (argc > 1) {
		std::cout << argv[1] << " ";
	}

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
	viewing_plane.set_orientation(
		glm::vec3(-3, 50, -3),
		glm::vec3(20, 45, 0),
		glm::vec3(CANVAS_WIDTH / 2.0f * CAMERA_SCALE, CANVAS_HEIGHT / 2.0f * CAMERA_SCALE, 1.0f)
	);
	display_func();
	display_func();
	// Start the main loop
	glutMainLoop();
	return 0;
}
