#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define VPADDING 150
#define RECT_WIDTH 5
#define SPACE 1


/* Global variables */

FT_Library ft_library;
FT_Face ft_face;

struct Algo {
	char name[50];
	void (*function)();
};

struct Algo algos[2];

int selected_algo = 0;
int speed = 1;
int refresh_counter = 0;
int iter_counter = 0;
int arr_size;

float *arr;

bool run;


/* Algorithms */

// Just some variables to store the state of the running algorithm
struct AlgoState {
	int a;
	int b;
	int c;
};

struct AlgoState as = {0, 0, 0};


void swap_elements(int x, int y) {
	float temp = arr[x];
	arr[x] = arr[y];
	arr[y] = temp;
}


void bubble_sort() {

	/*
	 * a: Index of the current selection
	 * b: Index boundary of the sorted array
	 */

	if (as.a < arr_size - 1 - as.b) {
		if (arr[as.a] > arr[as.a + 1]) {
			swap_elements(as.a + 1, as.a);
		}

		as.a++;
	} else {
		as.b++;
		as.a = 0;
	}
}


void selection_sort() {

	/*
	 * a: Index of current selection
	 * b: Index of boundary of sorted array
	 * c: Index of the minimum element
	 */
	

	if (as.a < arr_size) {
		if (arr[as.a] < arr[as.c]) {
			
			// Save new minimum
			as.c = as.a;
		}

		as.a++;
	} else {
		swap_elements(as.b, as.c);

		as.b++;
		as.a = as.b;
		as.c = as.a;
	}
}


/* Helper functions */

void create_array() {
	arr_size = WINDOW_WIDTH / (RECT_WIDTH + SPACE);
	arr = (float*)malloc(arr_size * sizeof(float));

	float rect_increase = (WINDOW_HEIGHT - VPADDING * 2) / (float)(arr_size - 1);

	for (int i = 1; i <= arr_size; i++) {
		arr[i - 1] = i * rect_increase;
	}
}


void randomize_array() {
	srand(time(NULL));

	// Fisher-Yates shuffle
	for (int i = arr_size - 1; i > 0; i--) {
		int j = rand() % (i + 1);

		// Swap
		float temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}


bool array_sorted() {
	for (int i = 0; i < arr_size - 1; i++) {
		if (arr[i] > arr[i + 1]) {
			return false;
		}
	}

	return true;
}


void algo_selector(int direction) {
	int selection = selected_algo + direction;
	int lower = 0;
	int upper = (sizeof(algos) / sizeof(algos[0])) - 1;

	if (selection >= lower && selection <= upper) {
		selected_algo = selection;
	}
}




/* Render functions */

void render_text(int x, int y, char* text) {
	for (const char *c = text; *c; c++) {
	
		// Get glyph index from character code
		FT_UInt glyph_index = FT_Get_Char_Index(ft_face, *c);

		if (glyph_index == 0) {
			fprintf(stderr, "Given character code has no glyph image in the face\n");
			exit(1);
		}

		// Load glyph image
		if (FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT)) {
			fprintf(stderr, "Failed to load glyph.\n");
			exit(1);
		}

		// Render glyph
		if (FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL)) {
			fprintf(stderr, "Failed to render glyph.\n");
			exit(1);
		}

		FT_GlyphSlot slot = ft_face->glyph;
		FT_Bitmap* glyph_bitmap = &slot->bitmap;

		// Flip the bitmap vertically
		unsigned char* flipped_bitmap = (unsigned char*)malloc(glyph_bitmap->width * glyph_bitmap->rows);

		for (int row = 0; row < glyph_bitmap->rows; row++) {
			unsigned char* src_row = glyph_bitmap->buffer + (row * glyph_bitmap->width);
			unsigned char* dest_row = flipped_bitmap + ((glyph_bitmap->rows - row - 1) * glyph_bitmap->width);
			memcpy(dest_row, src_row, glyph_bitmap->width);
		}

        glyph_bitmap->buffer = flipped_bitmap;

        // Calculate the adjusted y position based on the glyph's bearing
        int adjusted_y = y + (slot->bitmap_top - glyph_bitmap->rows);
	
		glRasterPos2f(x, adjusted_y);
		glDrawPixels(glyph_bitmap->width, glyph_bitmap->rows, GL_LUMINANCE, GL_UNSIGNED_BYTE, glyph_bitmap->buffer);

		x += slot->advance.x / 64;
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);

	int x = 0;
	for (int i = 0; i < arr_size; i++) {

		// Bottom left
		glVertex2f(x, VPADDING);

		// Top left
		glVertex2f(x, VPADDING + arr[i]);

		// Top right
		glVertex2f(x + RECT_WIDTH, VPADDING + arr[i]);

		// Bottom right
		glVertex2f(x + RECT_WIDTH, VPADDING);

		x += RECT_WIDTH + SPACE;
	}

	glEnd();

	// Render text
	char text[256];

	// Top: Column 1
	sprintf(text, "Algorithm: %s", algos[selected_algo].name);
	render_text(20, WINDOW_HEIGHT - 50, text);
	
	sprintf(text, "Speed: %i", speed);
	render_text(20, WINDOW_HEIGHT - 80, text);
	
	// Top: Column 2
	sprintf(text, "Number of elements: %i", arr_size);
	render_text(500, WINDOW_HEIGHT - 50, text);

	sprintf(text, "Iterations: %i", iter_counter);
	render_text(500, WINDOW_HEIGHT - 80, text);


	// Bottom: Column 1
	render_text(20, VPADDING - 50, "Press a or s to select an algorithm.");
	render_text(20, VPADDING - 80, "Press u or d to modify speed.");
	render_text(20, VPADDING - 110, "Press r to randomize the array.");

	// Bottom: Column 2
	render_text(800, VPADDING - 50, "Press enter to run the algorithm.");

	glutSwapBuffers();
}


/* Refresh function */

void idle() {
	if (run) {
		algos[selected_algo].function();
		refresh_counter++;
		iter_counter++;

		if (refresh_counter == speed) {
			glutPostRedisplay();
			refresh_counter = 0;
		}

	} else {
		glutPostRedisplay();
	}

	if (array_sorted()) {
		run = false;
	}
}


/* User input handler */

void keyboard(unsigned char key, int x, int y) {

	// s: Next algorithm
	if (key == 115) {
		algo_selector(1);
	}

	// a: Previous algorithm
	if (key == 97) {
		algo_selector(-1);
	}

	// r: Reset state
	if (key == 114) {
		randomize_array();

		// Reset state
		iter_counter = 0;
		refresh_counter = 0;
		run = false;

		// Reset algo steps
		as = (struct AlgoState){0, 0};
	}
	
	// u: Increase speed
	if (key == 117) {
		speed++;
	}

	// d: reduce speed
	if (key == 100) {
		if (speed > 1) {
			speed--;
		}
	}
	
	// enter: Run program
	if (key == 13) {
		run = true;
	}

	// p: Pause program
	if (key == 112) {
		run = false;
	}

	
}


/* Set up functions */

void setup_gl() {

	// Set background dark
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Set point color and size to 1 pixel
	glColor3f(1.0, 0.7569, 0.0);
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

	/* 
	 * This fucking line... I spent a day rendering weird symbols
	 * because the padding that adds FreeType to each row of the bitmap
	 * does not match the padding expected by GL.
	 */

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}


void setup_freetype() {

	// Init library
	if (FT_Init_FreeType(&ft_library)) {
		fprintf(stderr, "Failed to initialize FreeType library\n");
		exit(1);
	}

	// Load font
	if (FT_New_Face(ft_library, "fonts/JetBrainsMono-Regular.ttf", 0, &ft_face)) {
		fprintf(stderr, "Failed to load font\n");
		exit(1);
	}

	// Set font size
	if (FT_Set_Pixel_Sizes(ft_face, 0, 24)) {
		fprintf(stderr, "Failed to set font size.\n");
		FT_Done_Face(ft_face);
		FT_Done_FreeType(ft_library);

		exit(1);
	}
}


int main(int argc, char** argv) {
	strcpy(algos[0].name, "Bubble sort");
	algos[0].function = &bubble_sort;

	strcpy(algos[1].name, "Selection sort");
	algos[1].function = &selection_sort;
	
	create_array();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Algorithm animator");

	setup_gl();
	setup_freetype();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	free(arr);

	FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_library);

	return 0;
}
