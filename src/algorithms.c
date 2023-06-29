#include "algorithms.h"


/* Bubble sort */

void *bubble_sort(void *arguments) {
	struct AlgoArgs *args = (struct AlgoArgs *)arguments;

	for (int step = 0; step < args->arr_size - 1; step++) {
		int swapped = false;

		for (int i = 0; i < args->arr_size - step - 1; i++) {
			args->comparisons++;
			args->arr[i].current = true;
			args->arr[i + 1].current = true;

			if (args->arr[i].value > args->arr[i + 1].value) {
				swap_elements(i, i + 1, args->arr);
				swapped = true;
			}

			control_flow(args->delay, args->sequentially, &args->pause);
			args->arr[i].current = false;
			args->arr[i + 1].current = false;
		}

		if (swapped == 0) {
			break;
		}
	}
}


/* Selection sort */

void *selection_sort(void *arguments) {
	struct AlgoArgs *args = (struct AlgoArgs *)arguments;

	for (int step = 0; step < args->arr_size - 1; step++) {
		int min_idx = step;

		for (int i = step + 1; i < args->arr_size; i++) {
			args->comparisons++;
			args->arr[i].current = true;
			args->arr[min_idx].current = true;

			control_flow(args->delay, args->sequentially, &args->pause);
			args->arr[i].current = false;
			args->arr[min_idx].current = false;

			if (args->arr[i].value < args->arr[min_idx].value) {
				min_idx = i;
			}

		}

		swap_elements(min_idx, step, args->arr);
	}
}


/* Quick sort */


