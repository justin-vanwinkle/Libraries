/*
 * PriorityQueue.h
 *
 *  Created on: Jun 23, 2014
 *      Author: lugoh008
 */

#ifndef PRIORITYQUEUE_H_
#define PRIORITYQUEUE_H_

#include "Objects.h"
#include "DArray.h"
#include "DoubleLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static int globalPriority = 0;

typedef enum {
	PQMODE_PQUEUE,
	PQMODE_QUEUE,
	PQMODE_STACK,
	PQMODE_BAG
} PQMode;

typedef struct _PriorityQueue {
	DoubleLinkedList* elements;
	ArrayList* elementSampling;
	PQMode mode;
	void (*priorityExtractor)(Object, Object);
} PQueue;

PQueue* allocPQueue(uint, PQMode);
void releasePQueue(PQueue*);
Object peekMin(PQueue*);
Object peekMax(PQueue*);
void removeMin(PQueue*);
void removeMax(PQueue*);
void add(PQueue*, Object);
bool isEmptyPQueue(PQueue*);

//stack and queue functions (when mode is stack or queue)
Object top(PQueue*);
void pop(PQueue*);
void push(PQueue*, Object);
Object front(PQueue*);
void dequeue(PQueue*);
void enqueue(PQueue*, Object);
Object peek(PQueue*);
void take(PQueue*);
void deposit(PQueue*);

void stackPriority(Object, Object);
void queuePriority(Object, Object);

void setComparatorPQueue(PQueue*, void (*extractor)(Object, Object));
void setPrinterPQueue(PQueue*, void (*printerFunction)(FILE*, Object));
void setReleasePQueue(PQueue*, void (*releaseFunction)(Object));

void printPQueue(FILE*, PQueue*);
void debugPQueue(FILE*, PQueue*);

void testPQueue(int, char**);

#endif /* PRIORITYQUEUE_H_ */
