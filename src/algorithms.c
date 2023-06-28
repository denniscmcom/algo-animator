#include "algorithms.h"


void *bubble_sort(void *arguments) {
	struct AlgoArgs *args = (struct AlgoArgs *)arguments;

	printf("Size: %i\n", args->arr_size);
	printf("Comparisons: %i\n", args->comparisons);
	(args->comparisons)++;
	printf("Comparisons: %i\n", args->comparisons);
	/*
	for (int i = 0; i < args->arr_size - 1; i++) {
		//(*args->comparisons)++;

		for (int j = 0; j < args->arr_size - i - 1; j++) {
			//(*args->comparisons)++;
			
			if (args->arr[j].value > args->arr[j + 1].value) {
				//swap_elements(j, j + 1, args->arr);
			}
			
			
			//usleep((*args->delay));
			//bool test = false;
			//delay_flow(args->delay, &test);
		}
	}
	*/
}
