#include "utils.h"
#include <stdio.h>
#include <GL/glut.h>


#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define RECT_WIDTH 5
#define SPACE 5


struct Rectangle {
	int width;
	int height;
	int x;
};

// Globals
struct Rectangle* rectangles;
int n_rectangles;
int test_counter = 0;


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


void render_text(int x, int y, char* text) {
	glRasterPos2f(x, y);

	for (const char *c = text; *c; ++c) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);

	for (int i = 0; i < n_rectangles; i++) {
		glVertex2f(rectangles[i].x, WINDOW_HEIGHT - 100);
		glVertex2f(rectangles[i].x + rectangles[i].width, WINDOW_HEIGHT - 100);
		glVertex2f(rectangles[i].x + rectangles[i].width, rectangles[i].height);
		glVertex2d(rectangles[i].x, rectangles[i].height);
	}

	glEnd();

	// Render text
	char text[256];
	sprintf(text, "Number of elements: %i", n_rectangles);
	render_text(40.0, WINDOW_HEIGHT - 50, text);

	sprintf(text, "Test counter: %i", test_counter);
	render_text(540.0, WINDOW_HEIGHT - 50, text);

	glutSwapBuffers();
	glFlush();
}


void idle() {
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {

	// 13 is the ASCII value of ENTER key
	if (key == 13) {
		test_counter++;
	}
}


int main(int argc, char** argv) {
	n_rectangles = floor((WINDOW_WIDTH - RECT_WIDTH) / (RECT_WIDTH + SPACE)) + 1;	
	rectangles = malloc(n_rectangles * sizeof(struct Rectangle));

	int x_pos = 1;

	int i = 0;
	while (i < n_rectangles) {
		rectangles[i].width = RECT_WIDTH;
		rectangles[i].height = random_int(100, WINDOW_HEIGHT - 100);
		rectangles[i].x = x_pos;

		x_pos += RECT_WIDTH + SPACE;
		i++;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL Window");
	setup();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	free(rectangles);

	return 0;
}


