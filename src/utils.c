#include "utils.h"


void create_array(struct Element *arr, int arr_size, int window_height, int vpadding) {
	float rect_increase = (window_height - vpadding * 2) / (float)(arr_size - 1);
	for (int i = 1; i <= arr_size; i++) {
		arr[i - 1].value = i * rect_increase;
		arr[i - 1].current = false;
	}
}


void swap_elements(int x, int y, struct Element *arr) {
	struct Element temp = arr[x];
	arr[x] = arr[y];
	arr[y] = temp;
}


void randomize_array(struct Element *arr, int arr_size) {
	srand(time(NULL));

	// Fisher-Yates shuffle
	for (int i = arr_size - 1; i > 0; i--) {
		int j = rand() % (i + 1);

		// Swap
		swap_elements(i, j, arr);
	}
}


void algorithm_selector(struct Algo *algos, int size, int direction, int *selected_algo) {
	int selection = *selected_algo + direction;

	if (selection >= 0 && selection <= size - 1) {
		*selected_algo = selection;
	}
}


void change_delay(struct AlgoArgs *algo_args, int change) {
	int new_delay = algo_args->delay + change;

	if (new_delay) {
		algo_args->delay = new_delay;
	}
}


void control_flow(useconds_t delay, bool sequentially, bool *pause) {
	if (sequentially) {
		*pause = true;
	}

	while (*pause) {
		// Wait to resume
	}

	usleep(delay);
}


void reset_state(struct AlgoArgs *algo_args, struct ThreadState *thread_state) {
	if (thread_state->running) {
		pthread_cancel(thread_state->thread);
	}

	randomize_array(algo_args->arr, algo_args->arr_size);

	algo_args->comparisons = 0;
	algo_args->pause = false;
	algo_args->sequentially = false;

	for (int i = 0; i < algo_args->arr_size - 1; i++) {
		algo_args->arr[i].current = false;
	}
}


void run(struct AlgoArgs *algo_args, struct Algo *algos, int selected_algo, 
		struct ThreadState *thread_state) {

	if (algo_args->pause) {
		algo_args->pause = false;
	} else {
		thread_state->running = true;
		pthread_create(&(thread_state->thread), NULL, algos[selected_algo].function, 
				(void *)algo_args);
	}
}

