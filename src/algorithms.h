#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "utils.h"


struct AlgoArgs {
	struct Element *arr;
	int arr_size;
	int *comparisons;
	useconds_t *delay;
	bool pause;
};

void *bubble_sort(void *arguments);


#endif // ALGORITHMS_H
