#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920


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
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
}


struct Rectangle {
	int width;
	int height;
	int x_position;
};


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);

	float x_position = 1;
	float rect_width = 5.0;
	float space = 5.0;

	// Compute max number of rectangles to fit the windows
	int max_rects = floor((WINDOW_WIDTH - rect_width) / (rect_width + space)) + x_position;	

	struct Rectangle *rectangles = malloc(max_rects * sizeof(struct Rectangle));
	int rect_counter = 0;

	while (rect_counter < max_rects) {
		struct Rectangle rectangle;
		rectangle.width = rect_width;
		rectangle.height = random_int(100, WINDOW_HEIGHT - 100);
		rectangle.x_position = x_position;
		rectangles[rect_counter] = rectangle;

		draw_rectangle(
				rectangle.x_position, 
				rectangle.height, 
				rectangle.width, 
				WINDOW_HEIGHT
		);

		x_position += rect_width + space;
		rect_counter++;
	}

	glEnd();

	char text[256];
	sprintf(text, "Number of elements: %i", rect_counter);
	render_text(text, 20.0, WINDOW_HEIGHT - 50);	

	free(rectangles);
	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL Window");
	setup();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
