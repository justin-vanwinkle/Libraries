/*
 * DArray.h
 *
 *  Created on: May 21, 2014
 *      Author: lugoh008
 */

#ifndef DARRAY_H_
#define DARRAY_H_

#define DEBUG 0

#include "Objects.h"
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<stdbool.h>
#include<string.h>

typedef struct DArray{
	GenericArray data;
	uint elementSize;
	uint maxElements;
	uint length;
	void (*releaseFunction)(Object);
	void (*printerFunction)(FILE*, Object);
} ArrayList;

void setReleaseDArray(ArrayList*, void (*releaser)(Object));
void setPrinterDArray(ArrayList*, void (*printer)(FILE*, Object));

/**
 * Allocates size elements space instead of 10.
 */
ArrayList* allocDArray(uint size, uint esize);
/**
 * Free the memory space so that it can be reused later.
 * We free stuff from bottom up so that we dont loose track of the memory space before the free.
 * We also must check there is something allocated, otherwise our program can crash.
 */
void releaseDArray(ArrayList* array);
/**
 * Adds an element at pos of the DArray.
 */
void insertDArray(ArrayList* array, uint pos, Object b);
/**
 * Adds an element at the end of the DArray.
 */
void appendDArray(ArrayList* array, Object b);
/**
 * Remove the element at position pos, and return it.
 */
void removeDArray(ArrayList* array, uint pos);
/**
 * Set an element at a given position pos to newValue, and return the oldValue.
 */
void setDArray(ArrayList* array, uint pos, Object newValue);
Object getDArray(ArrayList* array, uint pos);
/**
 * Returns true if the value is included in the array, false otherwise.
 */
bool containsDArray(ArrayList* array, Object value);
/**
 * Prints the content of the array as a Set. Also prints its length and count values.
 */
void printDArray(FILE* out, ArrayList* array);
void debugDArray(FILE* out, ArrayList* array);

int testDArray(int argc, char** argv);

#endif /* DARRAY_H_ */
