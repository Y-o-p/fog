// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>


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
static constexpr auto SAMPLE_PERIOD = 0.5f;

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

static float time_elapsed = 0.0f;
static float rotation_x = 0.0f;
static float rotation_y = 0.0f;
static float rotation_z = 0.0f;
static vec3 light_pos = vec3(0);
static auto timer = Timer();

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
		glm::vec3(-3, 50, -3),
		glm::vec3(20, 45, 0),
		glm::vec3(CANVAS_WIDTH / 2.0f * CAMERA_SCALE, CANVAS_HEIGHT / 2.0f * CAMERA_SCALE, 1.0f)
	);
	renderer.set_light_pos(&light_pos);
	renderer.set_view(&viewing_plane);
	renderer.render();
	glFlush();
}

void update(int ID) {
	if (timer.times_length() < 60) {
		timer.start();
		time_elapsed += UPDATE_RATE / 1000.0;
		
		display_func();

		glutTimerFunc(UPDATE_RATE, update, 0);
		double duration = timer.end();
		//std::printf("%fms | %f FPS            \n", duration * 1000.0, 1.0 / duration);
		//std::fflush(stdout);
	}
	else {
		// NOTE: Frames are being shown to the screen at an artifical constant rate.
		//		 However, actual rendering times may be faster.
		timer.output_results();
		exit(0);
	}
}

int main(int argc, char ** argv) {
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
	display_func();
	display_func();

	// Start the main loop
	glutMainLoop();
	return 0;
}
