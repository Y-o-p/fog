// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include "VolumeRenderer.h"

// Globals
#define CANVAS_WIDTH 400 
#define CANVAS_HEIGHT 400
#define CENTER_X CANVAS_WIDTH / 2
#define CENTER_Y CANVAS_HEIGHT / 2

using namespace glm;
 
auto viewing_plane = ViewingPlane<CANVAS_WIDTH, CANVAS_HEIGHT>();
auto renderer = VolumeRenderer<CANVAS_WIDTH, CANVAS_HEIGHT>();

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
	renderer.draw(viewing_plane);

	double duration = timer.end();
	std::printf("That took %f seconds\n", duration / 1000.0);
	glFlush();

	std::cout << "Done" << std::endl;
}

void update_func(int ID) {
	display_func();
}

int main(int argc, char ** argv) {
	viewing_plane.set_orientation(glm::vec3(-128, 128, -128), glm::vec3(20, 45, 0));
	
	// Initial setup
	glutInit(&argc, argv);
	my_setup(CANVAS_WIDTH, CANVAS_HEIGHT, "Fog");
	
	// Set up the callbacks
	glutDisplayFunc(display_func);
	//glutTimerFunc(50, update_func, 0);

	// Start the main loop
	glutMainLoop();
	return 0;
}
