// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <GL/glew.h>
#include <iostream>
#include <GL/freeglut.h>
#include <cmath>

#include "OpenGL445Setup.h"
#include "Volume.h"

// Globals
#define CANVAS_WIDTH 720 
#define CANVAS_HEIGHT 480

glm::vec3 point_buffer[CANVAS_WIDTH * CANVAS_HEIGHT];
Volume perlin = create_perlin_volume(256);

VoxelVertex get_voxel_vertex(glm::ivec3 p) {
	if (p.x < 0 || p.x > 255 ||
		p.y < 0 || p.y > 255 ||
		p.z < 0 || p.z > 255) {
		return VoxelVertex {
			0.0f,
			glm::vec3(0.0f),
		};
	}

	return perlin.get_voxel(p.x, p.y, p.z);
}

Voxel get_nearest_voxel(glm::vec3 p) {
	glm::ivec3 points[8] = {
		glm::ivec3(floor(p.x), floor(p.y), floor(p.z)),
		glm::ivec3(floor(p.x), floor(p.y), ceil(p.z)),
		glm::ivec3(floor(p.x), ceil(p.y),  floor(p.z)),
		glm::ivec3(floor(p.x), ceil(p.y),  ceil(p.z)),
		glm::ivec3(ceil(p.x),  floor(p.y), floor(p.z)),
		glm::ivec3(ceil(p.x),  floor(p.y), ceil(p.z)),
		glm::ivec3(ceil(p.x),  ceil(p.y),  floor(p.z)),
		glm::ivec3(ceil(p.x),  ceil(p.y),  ceil(p.z)),
	};

	Voxel nearest_voxel;
	for (int i = 0; i < nearest_voxel.size(); i++) {
		nearest_voxel[i] = get_voxel_vertex(points[i]);
	}
	return nearest_voxel;
}

void calculate_ray(glm::vec3 start, glm::vec3 dir, int steps = 100, float step = 0.5f) {
	dir = glm::normalize(dir);
	for (float t = 0; t < steps; t += step) {
		glm::vec3 current_pos = start + t * dir;
		Voxel vox = get_nearest_voxel(current_pos);
		std::cout << vox[0].value << std::endl;
	}
}

void clear_screen() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
		point_buffer[i] = glm::vec3(0.0f);
	}
}

void display_func() {
	clear_screen();
	
	glBegin(GL_POINTS);
	for (int y = 0; y < CANVAS_HEIGHT; y++) {
		for (int x = 0; x < CANVAS_WIDTH; x++) {
			auto color = point_buffer[y * CANVAS_WIDTH + x];
			glColor3f(color.x, color.y, color.z);
			glVertex2i(x, y);
		}
	}
	glEnd();
	glFlush();
}

int main(int argc, char ** argv) {

	calculate_ray(glm::vec3(0), glm::vec3(1, 1, 1));

	// Initial setup
	glutInit(&argc, argv);
	my_setup(CANVAS_WIDTH, CANVAS_HEIGHT, "Fog");
	
	// Set up the callbacks
	display_func();
	glutDisplayFunc(display_func);

	// Start the main loop
	glutMainLoop();
	return 0;
}
