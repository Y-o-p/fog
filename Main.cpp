// g++ Main.cpp -lGL -lGLEW -lGLU -ldl -lglut

#include <GL/glew.h>
#include <iostream>
#include <GL/freeglut.h>
#include <cmath>

#include "OpenGL445Setup.h"
#include "PerlinNoise.h"
#include "Volume.h"

// Globals
#define CANVAS_WIDTH 720 /* width of canvas - you may need to change this */
#define CANVAS_HEIGHT 480 /* height of canvas - you may need to change this */

char canvas_Name[] = "Fog";
double angle = 0.0;
int texture_index = 0;
GLuint my_texture_ID;

void draw_screen() {
	
}

void display_func() {
	int sin_texture_index = (int)((std::cos(texture_index / 30.0f) + 1.0f) / 2.0f * 256.0f);
	//std::cout << sin_texture_index << std::endl;

	// Clears the screen and draws everything
	glBindTexture(GL_TEXTURE_2D, my_texture_ID);
	glActiveTexture(GL_TEXTURE0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -10.0);
	glTranslated(0.0, 0.0, sin_texture_index / 256.0f * 5.0f);
	
	float start_x = sin_texture_index % 16;
	start_x /= 16.0f;
	float start_y = sin_texture_index / 16;
	start_y /= 16.0f;
	float width = 1.0f / 16.0f;

	glBegin(GL_TRIANGLES);
	glTexCoord2d(start_x,         start_y); 		glVertex3d(-5, -5, 0);
    glTexCoord2d(start_x + width, start_y); 		glVertex3d(5, -5, 0);
    glTexCoord2d(start_x + width, start_y + width); glVertex3d(5, 5, 0);

    glTexCoord2d(start_x + width, start_y + width); glVertex3d(5, 5, 0);
    glTexCoord2d(start_x, 		  start_y + width); glVertex3d(-5, 5, 0);
    glTexCoord2d(start_x,         start_y); 		glVertex3d(-5, -5, 0);
	glEnd();
	glFlush();
}

void rotate_cube(int ID) {
	texture_index++;
	display_func();
	glutTimerFunc(50, rotate_cube, 1);
}

int main(int argc, char ** argv) {
	// Create perlin noise
	const siv::PerlinNoise perlin{ 123456u };
	auto noise = std::vector<Voxel>(256 * 256 * 256);
	for (int yy = 0; yy < 16; yy++) {
		for (int xx = 0; xx < 16; xx++) {
			int index_1 = yy * 256 * 4096 + xx * 256;

			for (int y = 0; y < 256; y++) {
				for (int x = 0; x < 256; x++) {
					int index_2 = index_1 + (y * 4096 + x);
					noise[index_2] = Voxel {
						(float)perlin.noise3D_01(x * 0.05f, y * 0.05f, (yy * 16 + xx) * 0.05f),
						glm::vec3(0, 0, 0),
					};
					//std::cout << i << ", " << j << ", " << k << " | " << noise[(i * 256 + j) * 256 + k] << std::endl;
				}
			}
		}
	}

	auto volume = Volume(std::move(noise), 256, 256, 256);

	// for (Voxel voxel : volume.m_data) {
	// 	std::cout << voxel.gradient.x << ", " << voxel.gradient.y << ", " << voxel.gradient.z << std::endl;
	// }

	auto volume_raw = volume.get_raw_values();

	// Initial setup
	glutInit(&argc, argv);
	my_setup(CANVAS_WIDTH, CANVAS_HEIGHT, canvas_Name);
	
	// Set up the callbacks
	display_func();
	glutDisplayFunc(display_func);
	glutTimerFunc(50, rotate_cube, 1);
	// glutKeyboardFunc(keyboard_func);

	// Set up textures
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &my_texture_ID);

	glBindTexture(GL_TEXTURE_2D, my_texture_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 4096, 4096, 0, GL_RED, GL_FLOAT, volume_raw.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Start the main loop
	std::cout << "Any Key Click Will Start" << std::endl;
	glutMainLoop();
	return 0;
}
