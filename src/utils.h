#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>


struct Element {
	float value;
	bool current;
};

struct Algo {
	char name[50];
	void *(*function)(void *);
};

void create_array(struct Element *arr, int arr_size, int window_height, int vpadding);
void swap_elements(int x, int y, struct Element *arr);
void randomize_array(struct Element *arr, int arr_size);
bool array_sorted(struct Element *arr, int arr_size);
void algorithm_selector(struct Algo *algos, int direction, int *selected_algorithm);
void delay_flow(useconds_t *delay, bool *pause);


#endif // UTILS_H
