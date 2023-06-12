#include <stdio.h>
#include <GL/glut.h>


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
	gluOrtho2D(-300, 300, 300, -300);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glRectf(100.0f, 100.0f, 200.0f, 200.0f);
	//	glVertex2i(1920/2, 1080/2);
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
