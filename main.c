#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>


#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define OFFSET 150
#define RECT_WIDTH 5
#define SPACE 1


// Globals
char* algos[] = {"Bubble sort", "Selection sort", "Insertion sort", "Quick sort"};
int selected_algo = 0;
int n_algos = sizeof(algos) / sizeof(algos[0]);

int* arr;
int arr_size;


// Algos
void bubble_sort() {
	for (int step = 0; step < arr_size - 1; step++) {

		for (int i = 0; i < arr_size - step - 1; i++) {
			int current = arr[i];
			int next = arr[i + 1];

			if (current > next) {
				arr[i + 1] = current;
				arr[i] = next;
			}
		}
	}
}


void create_array() {
	arr_size = floor((WINDOW_WIDTH - RECT_WIDTH) / (RECT_WIDTH + SPACE)) + 1;	
	arr = (int*)malloc(arr_size * sizeof(int));

	// srand(time(NULL));

	int min = OFFSET;
	int max = WINDOW_HEIGHT - OFFSET;

	for (int i = 0; i < arr_size; i++) {
		arr[i] = rand() % ((max - min) + 1) + min;
	}
}


void print_array() {
	for (int i = 0; i < arr_size; i++) {
		printf("%d ", arr[i]);
	}
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

	/* 
	 * Creates projection matrix
	 * x increases from left to right (0 to WINDOW_WIDTH)
	 * y increases from bottom to top (0 to WINDOW_HEIGHT)
	 */
	gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

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

	int x = 0;
	for (int i = 0; i < arr_size; i++) {

		// Bottom left
		glVertex2f(x, OFFSET);

		// Top left
		glVertex2f(x, arr[i]);

		// Top right
		glVertex2f(x + RECT_WIDTH, arr[i]);

		// Bottom right
		glVertex2f(x + RECT_WIDTH, OFFSET);

		x += RECT_WIDTH + SPACE;
	}

	glEnd();

	// Render text
	char text[256];

	sprintf(text, "Algorithm: %s", algos[selected_algo]);
	render_text(20, OFFSET - 50, text);

	sprintf(text, "Number of elements: %i", arr_size);
	render_text(20, OFFSET - 75, text);

	render_text(WINDOW_WIDTH - 500, OFFSET - 50, "Press 'a' or 's' to select an algorithm");
	render_text(WINDOW_WIDTH - 500, OFFSET - 75, "Press 'enter' to run the algorithm");
	render_text(WINDOW_WIDTH - 500, OFFSET - 100, "Press 'r' to reset array");

	glutSwapBuffers();
	glFlush();
}


void idle() {
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {

	// s
	if (key == 115) {
		algo_selector(1);
	}

	// r
	if (key == 114) {
		create_array();
	}

	// a
	if (key == 97) {
		algo_selector(-1);
	}

	// enter
	if (key == 13) {
		printf("Before sorting: ");
		print_array();
		printf("\n\n");

		bubble_sort();

		printf("After sorting: ");
		print_array();
	}
}


int main(int argc, char** argv) {
	create_array();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL Window");
	setup();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	free(arr);

	return 0;
}


