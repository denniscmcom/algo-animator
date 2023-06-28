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


bool array_sorted(struct Element *arr, int arr_size) {
	for (int i = 0; i < arr_size - 1; i++) {
		if (arr[i].value > arr[i + 1].value) {
			return false;
		}
	}

	return true;
}


void algorithm_selector(struct Algo *algos, int direction, int *selected_algorithm) {
	int selection = *selected_algorithm + direction;
	int lower = 0;
	int upper = (int)((sizeof(algos) / sizeof(algos[0])) - 1);

	if (selection >= lower && selection <= upper) {
		*selected_algorithm = selection;
	}
}


void delay_flow(useconds_t *delay, bool *pause) {
	while (*pause) {
		// Wait to resume
	}
	
	usleep(*delay);
}
