// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <fstream>
#include <sstream>
#include <iomanip>

#include <glm/gtc/type_ptr.hpp>


// Globals
#define CANVAS_WIDTH 400 
#define CANVAS_HEIGHT 400
#define CENTER_X CANVAS_WIDTH / 2
#define CENTER_Y CANVAS_HEIGHT / 2
#define FPS 60.0
#define UPDATE_RATE 1000.0 / FPS
#define CUBE_SIZE 60
#define CAMERA_SCALE 0.25

#define CPU_BASED

using namespace glm;
 
#ifdef CPU_BASED
#include "VolumeRendererCPU.h"
static Volume<CUBE_SIZE> perlin = create_perlin_volume<CUBE_SIZE>();
static VolumeRendererCPU<CUBE_SIZE> renderer = VolumeRendererCPU<CUBE_SIZE>();
#else
#include "VolumeRenderer.h"
static VolumeRenderer<CUBE_SIZE> renderer;
#endif

static ViewingPlane viewing_plane = ViewingPlane(CANVAS_WIDTH, CANVAS_HEIGHT);
#include "OpenGL445Setup.h"

static float time_elapsed = 0.0f;
static float rotation_x = 0.0f;
static float rotation_y = 0.0f;
static float rotation_z = 0.0f;
static vec3 light_pos = vec3(0);
static auto timer = Timer();
static auto frame_times = std::vector<double>();

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
		glm::vec3(-128, 112, -128),
		glm::vec3(20, 45, 0),
		glm::vec3(CANVAS_WIDTH / 2.0f * CAMERA_SCALE, CANVAS_HEIGHT / 2.0f * CAMERA_SCALE, 1.0f)
	);
	renderer.set_light_pos(&light_pos);
	renderer.set_view(&viewing_plane);
	renderer.render();
	glFlush();
}

void update(int ID) {
	if (frame_times.size() < 102) {
		timer.start();
		time_elapsed += UPDATE_RATE / 1000.0;
		
		display_func();

		glutTimerFunc(UPDATE_RATE, update, 0);
		double duration = timer.end();
		frame_times.push_back(duration);
		std::printf("%fms | %f FPS            \n", duration, 1.0 / (duration / 1000.0));
		std::fflush(stdout);
	}
	else {
		// NOTE: Frames are being shown to the screen at an artifical constant rate.
		//		 However, actual rendering times may be faster.
		frame_times.erase(frame_times.begin(), frame_times.begin() + 2);
		std::cout << frame_times.size() << std::endl;
		double total_ms = std::accumulate(frame_times.begin(), frame_times.end(), 0.0);
		double total_s = total_ms / 1000.0;
		double average_frame_time = total_ms / frame_times.size();
		auto variance_fun = [average_frame_time](auto accumulator, auto val){
			return accumulator + std::pow(val - average_frame_time, 2.0);
		};
		double variance = std::accumulate(frame_times.begin(), frame_times.end(), 0.0, variance_fun) / (frame_times.size() - 1);
		double average_fps = 1.0 / (average_frame_time / 1000.0);
		using namespace std;
		cout << endl;
		cout << setw(30) << setfill('.') << left << "Total rendering time" << right << total_s << "s" << endl; 
		cout << setw(30) << setfill('.') << left << "Mean frame time" << right << average_frame_time << "ms" << endl; 
		cout << setw(30) << setfill('.') << left << "Frame time variance" << right << variance << "ms" << endl; 
		cout << setw(30) << setfill('.') << left << "Mean FPS" << right << average_fps << "FPS" << endl; 
		exit(0);
	}
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
