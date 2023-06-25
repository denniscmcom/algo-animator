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
#define OFFSET 150
#define RECT_WIDTH 5
#define SPACE 1


/* Global variables */

FT_Library ft_library;
FT_Face ft_face;

char algos[4][50] = {
	"Bubble sort", 
	"Selection sort", 
	"Insertion sort", 
	"Quick sort"
};

int selected_algo = 0;
int refresh_counter = 0;
int iter_counter = 0;
int arr_size;

int* arr;

bool run;


/* Algorithms */

// Bubble sort
struct BubbleSortInfo {
	int step;
	int i;
};

struct BubbleSortInfo bs = {1, 0};

void bubble_sort() {
	if (bs.i < arr_size - bs.step - 1) {
		int current = arr[bs.i];
		int next = arr[bs.i + 1];

		if (current > next) {
			arr[bs.i + 1] = current;
			arr[bs.i] = next;
		}

		bs.i++;
	} else {
		bs.step++;
		bs.i = 0;
	}
}


/* Helper functions */

void create_array() {
	arr_size = floor((WINDOW_WIDTH - RECT_WIDTH) / (RECT_WIDTH + SPACE)) + 1;	
	arr = (int*)malloc(arr_size * sizeof(int));

	srand(time(NULL));

	int min = OFFSET;
	int max = WINDOW_HEIGHT - OFFSET;

	for (int i = 0; i < arr_size; i++) {
		arr[i] = rand() % ((max - min) + 1) + min;
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

unsigned char* flipBitmapVertically(unsigned char* bitmap, int width, int height) {
    unsigned char* flippedBitmap = (unsigned char*)malloc(width * height);

    for (int row = 0; row < height; row++) {
        unsigned char* srcRow = bitmap + (row * width);
        unsigned char* destRow = flippedBitmap + ((height - row - 1) * width);
        memcpy(destRow, srcRow, width);
    }

    return flippedBitmap;
}


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

		x += 15;
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
	render_text(20, WINDOW_HEIGHT - 50, text);

	sprintf(text, "Number of elements: %i", arr_size);
	render_text(20, WINDOW_HEIGHT - 80, text);

	sprintf(text, "Iterations: %i", iter_counter);
	render_text(20, WINDOW_HEIGHT - 110, text);

	render_text(20, OFFSET - 50, "Press a or s to select an algorithm");
	render_text(20, OFFSET - 80, "Press enter to run the algorithm");
	render_text(20, OFFSET - 110, "Press r to reset array");

	glutSwapBuffers();
}


/* Refresh function */

void idle() {
	if (run) {
		bubble_sort();
		refresh_counter++;
		iter_counter++;

		if (refresh_counter == 90) {
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

	// s
	if (key == 115) {
		algo_selector(1);
	}

	// a
	if (key == 97) {
		algo_selector(-1);
	}

	// r
	if (key == 114) {
		create_array();
		iter_counter = 0;
		refresh_counter = 0;
		run = false;
		bs = (struct BubbleSortInfo){0, 0};
	}

	// enter
	if (key == 13) {
		run = true;
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
