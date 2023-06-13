#include "utils.h"


void render_text(const char *text, float x, float y) {
	glRasterPos2f(x, y);

	for (const char *c = text; *c; ++c) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}


int random_int(int min, int max) {
	return rand() % ((max - min) + 1) + min;
}


void draw_rectangle(int x_position, int rect_height, int rect_width, int window_height) {
	glVertex2f(x_position, window_height - 100);
	glVertex2f(x_position + rect_width, window_height - 100);
	glVertex2f(x_position + rect_width, rect_height);
	glVertex2d(x_position, rect_height);
}
