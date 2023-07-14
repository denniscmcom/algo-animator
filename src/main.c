#include "algorithms.h"


int window_width = 1920;
int window_height = 1080;
int vpadding = 150;
int rect_width = 5;
int space = 1;

struct Algo algos[4];
int selected_algo = 0;
int algos_size;

struct AlgoArgs algo_args;
struct ThreadState thread_state;

FT_Library ft_library;
FT_Face ft_face;


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
		unsigned char* flipped_bitmap = (unsigned char*)malloc(
				glyph_bitmap->width * glyph_bitmap->rows);

		for (int row = 0; row < glyph_bitmap->rows; row++) {
			unsigned char* src_row = glyph_bitmap->buffer + (row * glyph_bitmap->width);
			unsigned char* dest_row = flipped_bitmap + ((glyph_bitmap->rows - row - 1) * 
					glyph_bitmap->width);

			memcpy(dest_row, src_row, glyph_bitmap->width);
		}

		glyph_bitmap->buffer = flipped_bitmap;

		// Calculate the adjusted y position based on the glyph's bearing
		int adjusted_y = y + (slot->bitmap_top - glyph_bitmap->rows);

		glRasterPos2f(x, adjusted_y);
		glDrawPixels(glyph_bitmap->width, glyph_bitmap->rows, GL_LUMINANCE, 
				GL_UNSIGNED_BYTE, glyph_bitmap->buffer);

		x += slot->advance.x / 64;
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);

	int x = 0;
	for (int i = 0; i < algo_args.arr_size; i++) {

		if (algo_args.arr[i].current) {
			glColor3f(1.0, 1.0, 1.0);
		} else {
			glColor3f(1.0, 0.7569, 0.0);
		}

		// Bottom left
		glVertex2f(x, vpadding);

		// Top left
		glVertex2f(x, vpadding + algo_args.arr[i].value);

		// Top right
		glVertex2f(x + rect_width, vpadding + algo_args.arr[i].value);

		// Bottom right
		glVertex2f(x + rect_width, vpadding);

		x += rect_width + space;
	}

	glEnd();

	// Render text
	char text[256];

	// Top: Column 1
	sprintf(text, "Algorithm: %s", algos[selected_algo].name);
	render_text(20, window_height - 50, text);

	sprintf(text, "Delay: %u microseconds", algo_args.delay);
	render_text(20, window_height - 80, text);

	// Top: Column 2
	sprintf(text, "Number of elements: %i", algo_args.arr_size);
	render_text(500, window_height - 50, text);

	sprintf(text, "Comparisons: %i", algo_args.comparisons);
	render_text(500, window_height - 80, text);

	// Top: Column 3
	if (algo_args.pause && !algo_args.sequentially) {
		sprintf(text, "PAUSED");
		render_text(window_width - 400, window_height - 50, text);
	}

	if (algo_args.sequentially) {
		sprintf(text, "SEQUENTIAL MODE");
		render_text(window_width - 400, window_height - 80, text);
	}

	// Bottom: Column 1
	render_text(20, vpadding - 50, "Press a or s to select an algorithm.");
	render_text(20, vpadding - 80, "Press u or d to change the delay.");
	render_text(20, vpadding - 110, "Press r to reset.");

	// Bottom: Column 2
	render_text(800, vpadding - 50, "Press enter to run or resume the algorithm.");
	render_text(800, vpadding - 80, "Press p to pause.");
	render_text(800, vpadding - 110, "Press q to enable or disable sequential mode.");

	glutSwapBuffers();
}


void idle() {
	glutPostRedisplay();	
}


void keyboard(unsigned char key, int x, int y) {

	// s: Next algorithm
	if (key == 115) {
		algorithm_selector(algos, algos_size, 1, &selected_algo);
	}

	// a: Previous algorithm
	if (key == 97) {
		algorithm_selector(algos, algos_size, -1, &selected_algo);
	}

	// r: Reset state
	if (key == 114) {
		reset_state(&algo_args, &thread_state);
	}

	// u: Increase delay
	if (key == 117) {
		change_delay(&algo_args, 10);
	}

	// d: reduce delay
	if (key == 100) {
		change_delay(&algo_args, -10);
	}

	// enter: Run program
	if (key == 13) {
		run(&algo_args, algos, selected_algo, &thread_state);
	}

	// p: Pause program
	if (key == 112) {
		algo_args.pause = true;
	}

	// q: Enable sequential mode
	if (key == 113) {
		algo_args.sequentially = !algo_args.sequentially;
	}
}


void setup_gl() {

	// Set background dark
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Set point color and size
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

	gluOrtho2D(0, window_width, 0, window_height);

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


int main(int argc, char *argv[]) {
	if (argc - 1 == 4) {
		int args[4];

		printf("Values provided: ");	
		
		for (int i = 1; i < argc; i++) {
			printf("%s ", argv[i]);

			char* end_ptr;
			long value = strtol(argv[i], &end_ptr, 10);

			if (end_ptr == argv[i]) {
				printf("Invalid argument\n");
				exit(1);
			}

			if ((value == LONG_MAX || value == LONG_MIN)) {
				printf("Integer out of range\n");
				exit(1);
			}

			args[i - 1] = (int)value;
		}

		printf("\n");

		printf("Window width: %i\n", args[0]);
		window_width = args[0];

		printf("Window height: %i\n", args[1]);
		window_height = args[1];

		printf("Rectangle width: %i\n", args[2]);
		rect_width = args[2];

		printf("Space: %i\n", args[3]);
		space = args[3];

	} else {
		printf("Using default values\n");
	}

	algo_args.arr_size = window_width / (rect_width + space);
	algo_args.arr = malloc(algo_args.arr_size * sizeof(struct Element));
	algo_args.comparisons = 0;
	algo_args.pause = false;
	algo_args.sequentially = false;
	algo_args.delay = 100;

	strcpy(algos[0].name, "Bubble sort");
	algos[0].function = bubble_sort;

	strcpy(algos[1].name, "Selection sort");
	algos[1].function = selection_sort;

	strcpy(algos[2].name, "Quick sort");
	algos[2].function = quick_sort;

	strcpy(algos[3].name, "Insertion sort");
	algos[3].function = insertion_sort;
	
	algos_size = sizeof(algos) / sizeof(algos[0]);

	create_array(algo_args.arr, algo_args.arr_size, window_height, vpadding);
	randomize_array(algo_args.arr, algo_args.arr_size);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Visualization of sorting algorithms");

	setup_gl();
	setup_freetype();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	free(algo_args.arr);

	FT_Done_Face(ft_face);
	FT_Done_FreeType(ft_library);

	return 0;
}
