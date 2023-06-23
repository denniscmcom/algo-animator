#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>


#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define RECT_WIDTH 5
#define SPACE 5


// Globals
char* algos[] = {"Bubble sort", "Selection sort", "Insertion sort", "Quick sort"};

int selected_algo = 0;
int n_algos = sizeof(algos) / sizeof(algos[0]);
int n_rectangles;

struct Rectangle {
	int width;
	int height;
	int x;
};

struct Rectangle* rectangles;


// Algos
void bubble_sort() {
	for (int i = 0; i < n_rectangles; i++) {
		
		struct Rectangle current = rectangles[i];
		struct Rectangle next = rectangles[i + 1];

		printf("Current pos: %i\n", current.x);
		printf("Next pos: %i\n", next.x);

		// Place largest element at the end of the array
		if (current.height > next.height) {
			rectangles[i + 1].x = current.x;
			rectangles[i].x = next.x;
		}
	}
}


// Utils
int random_int(int min, int max) {
	return rand() % ((max - min) + 1) + min;
}


void algo_selector(int direction) {
	int selection = selected_algo + direction;
	int lower = 0;
	int upper = (sizeof(algos) / sizeof(algos[0])) - 1;

	if (selection >= lower && selection <= upper) {
		selected_algo = selection;
	}
}

// GL and GLUT
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
		glVertex2f(rectangles[i].x, WINDOW_HEIGHT - 200);
		glVertex2f(rectangles[i].x + rectangles[i].width, WINDOW_HEIGHT - 200);
		glVertex2f(rectangles[i].x + rectangles[i].width, rectangles[i].height);
		glVertex2d(rectangles[i].x, rectangles[i].height);
	}

	glEnd();

	// Render text
	char text[256];

	sprintf(text, "Algorithm: %s", algos[selected_algo]);
	render_text(20.0, WINDOW_HEIGHT - 130, text);

	sprintf(text, "Number of elements: %i", n_rectangles);
	render_text(20.0, WINDOW_HEIGHT - 100, text);

	render_text(WINDOW_WIDTH - 500, WINDOW_HEIGHT - 130, "Press 'a' or 's' to select an algorithm");
	render_text(WINDOW_WIDTH - 500, WINDOW_HEIGHT - 100, "Press 'enter' to run the algorithm");

	glutSwapBuffers();
	glFlush();
}


void idle() {
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {

	// S
	if (key == 115) {
		algo_selector(1);
	}

	// A
	if (key == 97) {
		algo_selector(-1);
	}

	// Enter
	if (key == 13) {
		bubble_sort();
	}
}


int main(int argc, char** argv) {
	n_rectangles = floor((WINDOW_WIDTH - RECT_WIDTH) / (RECT_WIDTH + SPACE)) + 1;	
	rectangles = malloc(n_rectangles * sizeof(struct Rectangle));

	int x_pos = 1;

	int i = 0;
	while (i < n_rectangles) {
		rectangles[i].width = RECT_WIDTH;
		rectangles[i].height = random_int(100, WINDOW_HEIGHT - 200);
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


