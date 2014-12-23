/*
 * DArray.c
 *
 *  Created on: Jun 25, 2014
 *      Author: lugoh008
 */


#include "DArray.h"

/**
 * Allocates size elements space instead of 10.
 */
ArrayList* allocDArray(uint size, uint esize){//, void (*releaser(Object))){
	ArrayList* array;
	array = (ArrayList*)malloc(sizeof(ArrayList));
	array->data = (Object)calloc(size, esize);
	array->elementSize = esize;
	array->maxElements = size;	//holds the max capacity.
	array->length = 0;	//holds the number of current elements
	//array->
	return array;
}

void setReleaseDArray(ArrayList* array, void (*releaser)(Object)){
	array->releaseFunction = releaser;
}
void setPrinterDArray(ArrayList* array, void (*printer)(FILE*, Object)){
	array->printerFunction = printer;
}

/**
 * Free the memory space so that it can be reused later.
 * We free stuff from bottom up so that we dont loose track of the memory space before the free.
 * We also must check there is something allocated, otherwise our program can crash.
 */
void releaseDArray(ArrayList* array){
	if(array){
		if(array->data){
			uint i = 0;
			for(i = 0; i < array->length; ++i){
				array->releaseFunction(array->data[i]);
			}
		}
		free(array);
	}
}

/**
 * Adds an element at pos of the DArray.
 */
void insertDArray(ArrayList* array, uint pos, Object b){
	//increase size when the number of elements is equal to the size (no space for new data).
	if(array->length == array->maxElements){
#if DEBUG
		fprintf(stderr, "Warning: Size of dynamic array is increasing to %d\n", 2*array->maxElements);
#endif
		//2*array->size is twice more positions
		array->maxElements = 2*array->maxElements;
		//increase size
		array->data = realloc(array->data, array->maxElements*array->elementSize);
		int newPositions = array->length;
		for(; newPositions < array->maxElements; ++newPositions){
			array->data[newPositions] = NULL;
		}
	}
	int i = array->length - 1;
	int npos = pos;
	while(i >= npos){
		if(!array->data[i + 1]){
			array->data[i + 1] = malloc(array->elementSize);
		}
		memcpy((array->data[i + 1]), (array->data[i]), array->elementSize);
		--i;
	}
	memcpy(array->data[pos], b, array->elementSize);
	++array->length;
	//1) array->data[array->length] = b;
	//2) ++array->length;
}

/**
 * Adds an element at the end of the DArray.
 */
void appendDArray(ArrayList* array, Object b){
	//increase size when the number of elements is equal to the size (no space for new data).
	if(array->length == array->maxElements){
#if DEBUG
		fprintf(stderr, "Warning: Size of dynamic array is increasing to %d\n", 2*array->maxElements);
#endif
		//2*array->size is twice more positions
		array->maxElements = 2*array->maxElements;
		//increase size
		array->data = realloc(array->data, array->maxElements*array->elementSize);
		int newPositions = array->length;
		for(; newPositions < array->maxElements; ++newPositions){
			array->data[newPositions] = NULL;
		}
	}

	if(!array->data[array->length]){
		array->data[array->length] = malloc(array->elementSize);
		//calloc(1, array->elementSize);
	}

	memcpy(array->data[array->length], b, array->elementSize);
	++array->length;
	//1) array->data[array->length] = b;
	//2) ++array->length;
}



/**
 * Remove the element at position pos, and return it.
 */
void removeDArray(ArrayList* array, uint pos){
	//cant remove from an empty list
	if(array->length == 0){
#if DEBUG
		fprintf(stderr, "Error: Attempt to remove from an empty list\n");
#endif
		return;
	}
	if(pos >= 0 && pos < array->length){
		//can remove
		//free(array->data[pos]);
		//everything after pos must be shifted back
		//memcpy((array->data + pos), (array->data + pos + 1), (array->elementSize*(array->length - (1 + pos))));
		uint i = pos;
		while(i < array->length - 1){
			memcpy((array->data[i]), (array->data[i + 1]), array->elementSize);
			++i;
		}
		//then the last element is now one position behind
		//free(array->data[array->length]);
		--array->length;
	}
}

/**
 * Set an element at a given position pos to newValue, and return the oldValue.
 */
void setDArray(ArrayList* array, uint pos, Object newValue){
	if(array->length == 0){
#if DEBUG
		fprintf(stderr, "Error: Attempt to set an empty list\n");
#endif
		return;
	}
	if(pos >= 0 && pos < array->length){
		//can set
		memcpy(array->data[pos], newValue, array->elementSize);
	}
#if DEBUG
	fprintf(stderr, "Error: Invalid position to set\n");
#endif
}

Object getDArray(ArrayList* array, uint pos){
	if(array->length == 0){
#if DEBUG
		fprintf(stderr, "Error: Attempt to get from an empty list\n");
#endif
		return NULL;	//cannot set an empty list
	}
	if(pos >= 0 && pos < array->length){
		Object toReturn = malloc(array->elementSize);
		memcpy(toReturn, array->data[pos], array->elementSize);
		return toReturn;
	}
#if DEBUG
	fprintf(stderr, "Error: Invalid position to get, returning NULL\n", UNDEF);
#endif
	return NULL;	//this should never happen
}

/**
 * Returns true if the value is included in the array, false otherwise.
 */
bool containsDArray(ArrayList* array, Object value){
	int i = 0;
	for(i = 0; i < array->length; ++i){
		if(memcmp(array->data[i], value, array->elementSize) == 0){
			return true;
		}
	}
	//false is return if the array is run completely and value is never found.
	return false;
}

/**
 * Prints the content of the array as a Set. Also prints its length and count values.
 */
void printDArray(FILE* out, ArrayList* array){
	fprintf(out, "Dynamic Array Info:\nLength = %d\nSize = %d\nData = {", array->length, array->maxElements);
	int i = 0;
	for(i = 0; i < array->length; ++i){
		array->printerFunction(out, array->data[i]);
		if(i != array->length - 1){
			fprintf(out, ", ");
		}
	}
	fprintf(out, "}\n");
}

void debugDArray(FILE* out, ArrayList* array){
	uint size = array->length;
	uint i = 0;
	for(i = 0; i < size; ++i){
		fprintf(out, "%p", array->data[i]);
		array->printerFunction(out, array->data[i]);
		fprintf(out, "\t");
	}
}


int testDArray(int argc, char** argv){
	int x = 0;
	ArrayList* alist = allocDArray(10, sizeof(int));
	setPrinterDArray(alist, &printInt);
	setReleaseDArray(alist, &releasePrimitive);
	//0
	appendDArray(alist, &x);
	++x;
	//0, 1
	appendDArray(alist, &x);
	++x;
	//0, 1, 2
	appendDArray(alist, &x);
	++x;
	//0, 1, 2, 3
	appendDArray(alist, &x);
	printDArray(stdout, alist);
	debugDArray(stdout, alist);
	printf("\n");
	//1, 2, 3
	removeDArray(alist, 0);
	printDArray(stdout, alist);
	printf("\n");
	debugDArray(stdout, alist);
	printf("\n");
	++x;
	//1, 2, 3, 4
	appendDArray(alist, &x);
	printDArray(stdout, alist);
	++x;
	//1, 2, 3, 4, 5
	appendDArray(alist, &x);
	printDArray(stdout, alist);
	//1
	printf("First element is %d\n", *((int*)getDArray(alist, 0)));
	//5, 2, 3, 4, 5
	setDArray(alist, 0, &x);
	//5
	printf("First element is %d\n", *((int*)getDArray(alist, 0)));
	printDArray(stdout, alist);

	removeDArray(alist, 2);
	x = 2;
	setDArray(alist, 2, &x);
	printDArray(stdout, alist);

	x = 7;
	insertDArray(alist, 2, &x);
	printDArray(stdout, alist);

	return 0;
}
