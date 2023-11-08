#pragma once

#include <iostream>

int* LowerBound(int* first, int* last, int value) {
	int* current;
	ptrdiff_t length, step;
	length = std::distance(first,last);
	while (length > 0){
		current = first;
		step = length/2;
		std::advance(current, step);
		if (*current < value){
			first = ++current;
			length -= step + 1;
		}
		else
			length = step;
	}
	return (*first == value? first : last);

}
