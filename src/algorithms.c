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

int qs_partition(struct AlgoArgs *args, int left, int right) {
	printf("Pivot index: %i\n", right);

	args->arr[right].current = true;
	float pivot = args->arr[right].value;
	control_flow(args->delay / 4, args->sequentially, &args->pause);
	
	int i = left - 1;

    for (int j = left; j < right; j++) {
		args->comparisons++;
		args->arr[j].current = true;
		
		control_flow(args->delay / 4, args->sequentially, &args->pause);
        
		if (args->arr[j].value < pivot) {
			printf("Value at index %i is smaller than pivot\n", j);
            i++;	
			printf("Swap with value at index %i\n", i);

            args->arr[i].current = true;
			control_flow(args->delay / 4, args->sequentially, &args->pause);
			args->arr[j].current = false;
            args->arr[i].current = false;

			struct Element temp = args->arr[i];
			swap_elements(i, j, args->arr);

        } else {
			args->arr[j].current = false;
		}
    }
	
    
	printf("Swap pivot with value at index %i\n", i + 1);
	args->arr[i + 1].current = true;
	control_flow(args->delay / 4, args->sequentially, &args->pause);
	args->arr[right].current = false;
	args->arr[i + 1].current = false;
	
	struct Element temp = args->arr[i + 1];
	swap_elements(i + 1, right, args->arr);

	printf("Finished partition\n");

    return i + 1;
}


void *quick_sort(void *arguments) {
	struct AlgoArgs *args = (struct AlgoArgs *)arguments;
	
	int left = 0;
	int right = args->arr_size - 1;
    int stack[right + 1 - left];
    int top = -1;

    stack[++top] = left;
    stack[++top] = right;

    while (top >= 0) {
        right = stack[top--];
        left = stack[top--];

        int pivot_index = qs_partition(args, left, right);

        if (pivot_index - 1 > left) {
            stack[++top] = left;
            stack[++top] = pivot_index - 1;
        }

        if (pivot_index + 1 < right) {
            stack[++top] = pivot_index + 1;
            stack[++top] = right;
        }
    }
}


/* Insertion sort */

void *insertion_sort(void *arguments) {
	struct AlgoArgs *args = (struct AlgoArgs *)arguments;

	for (int step = 1; step < args->arr_size; step++) {
		struct Element key = args->arr[step];

		printf("Selected key at index: %i\n", step);
		args->arr[step].current = true;
		control_flow(args->delay / 3, args->sequentially, &args->pause);

		int i = step - 1;

		while (key.value < args->arr[i].value && i >= 0) {
			args->comparisons++;
			printf("Key value is smaller than value at index %i\n", i);
			
			args->arr[i].current = true;
			args->arr[i + 1].value = 0;
			control_flow(args->delay / 3, args->sequentially, &args->pause);
			args->arr[i].current = false;
			
			printf("Move foward\n");
			args->arr[i + 1] = args->arr[i];

			i--;
		}

		printf("Place key at correct position\n");
		args->arr[i + 1] = key;
		
		args->arr[i + 1].current = true;
		control_flow(args->delay / 3, args->sequentially, &args->pause);
		args->arr[i + 1].current = false;
		args->arr[step].current = false;
	}
}


/* Merge sort */

void *merge_sort(void *arguments) {
	struct AlgoArgs *args = (struct AlgoArgs *)arguments;
}
