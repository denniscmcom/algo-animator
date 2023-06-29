#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <GL/glut.h>
#include <pthread.h>
#include <ft2build.h>
#include <signal.h>
#include FT_FREETYPE_H


struct Element {
	float value;
	bool current;
};

struct Algo {
	char name[50];
	void *(*function)(void *);
};

struct AlgoArgs {
	struct Element *arr;
	int arr_size;
	int comparisons;
	bool pause;
	bool sequentially;
	useconds_t delay;
};


struct ThreadState {
	bool running;
	pthread_t thread;
};

void create_array(struct Element *arr, int arr_size, int window_height, int vpadding);
void swap_elements(int x, int y, struct Element *arr);
void randomize_array(struct Element *arr, int arr_size);
bool array_sorted(struct Element *arr, int arr_size);
void algorithm_selector(struct Algo *algos, int algos_size, int direction, int *selected_algo);
void change_delay(struct AlgoArgs *algo_args, int change);
void control_flow(useconds_t delay, bool sequentially, bool *pause);
void reset_state(struct AlgoArgs *algo_args, struct ThreadState *thread_state);
void run(struct AlgoArgs *algo_args, struct Algo *algos, int selected_algo, 
		struct ThreadState *thread_state);


#endif // UTILS_H 
