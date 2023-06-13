#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define HEIGHT 1080
#define WIDTH 1920


void setup() {

	// Set background dark
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Set point color and size to 1 pixel
	glColor3f(0.0, 1.0, 0.0);
	glPointSize(5.0);

	// Matrix projection and reset with identity
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the coordinates to be used with the viewport
	gluOrtho2D(0, WIDTH, HEIGHT, 0);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);

	float x = 1;
	float rect_width = 5.0;
	float space = 5.0;
	
	// Compute max number of rectangles to fit the windows
	int max_rects = floor((WIDTH - rect_width) / (rect_width + space)) + x;	
	
	// Initialize empty array with same of `max_rects`
	int *unsorted_array = (int*)malloc(max_rects * sizeof(int));

	int rect_counter = 0;
	while (rect_counter < max_rects) {
		int height = random_int(100, HEIGHT - 100);

		glVertex2f(x, HEIGHT - 100);
		glVertex2f(x + rect_width, HEIGHT - 100);
		glVertex2f(x + rect_width, height);
		glVertex2d(x, height);

		x += rect_width + space;
		rect_counter++;
	}

	glEnd();
	
	char text[256];
	sprintf(text, "Number of elements: %i", rect_counter);
    render_text(text, 20.0, HEIGHT - 50);	
	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("OpenGL Window");
	setup();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
