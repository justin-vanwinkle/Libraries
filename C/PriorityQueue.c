/*
 * PriorityQueue.c
 *
 *  Created on: Jun 23, 2014
 *      Author: lugoh008
 */


#include "PriorityQueue.h"

PQueue* allocPQueue(uint elementSize, PQMode mode){
	PQueue* pq = (PQueue*)calloc(1, sizeof(PQueue));
	pq->elements = allocDList(elementSize, NULL, NULL);

	//the elementSampling is to speed up search time, but it is not yet ready
	//in order for it to work I cannot make a copy of DNode*, rather I should
	//place the pointer of a DNode directly into the array
	//I will call this a DeepDArray
	pq->elementSampling = (GenericArray)allocDArray(10, sizeof(DNode*));

	pq->mode = mode;
	if(mode == PQMODE_STACK){
		pq->priorityExtractor = &stackPriority;
	}
	else if(mode == PQMODE_QUEUE){
		pq->priorityExtractor = &queuePriority;
	}
	return pq;
}
void releasePQueue(PQueue* pq){
	if(pq){
		if(pq->elements){
			releaseDList(pq->elements);
		}
		if(pq->elementSampling){
			releaseDArray(pq->elementSampling);
		}
		free(pq);
	}
}
Object peekMin(PQueue* pq){
	if(isEmptyPQueue(pq)){
		return NULL;
	}
	Object data = malloc(pq->elements->elementSize);
	memcpy(data, pq->elements->head->data, pq->elements->elementSize);
	return data;
}
Object peekMax(PQueue* pq){
	if(isEmptyPQueue(pq)){
		return NULL;
	}
	Object data = malloc(pq->elements->elementSize);
	memcpy(data, pq->elements->tail->data, pq->elements->elementSize);
	return data;
}
void removeMin(PQueue* pq){
	if(isEmptyPQueue(pq)){
		return;
	}
	Object toFree = NULL;
	toFree = removeDList(pq->elements, 0);
	if(toFree){
		//calling release function instead of just free
		//in case some other data type other than a primitive is
		//in PQueue
		pq->elements->releaseFunction(toFree);
	}
}
void removeMax(PQueue* pq){
	if(isEmptyPQueue(pq)){
		return;
	}
	Object toFree = NULL;
	toFree = removeDList(pq->elements, pq->elements->size - 1);
	if(toFree){
		//calling release function instead of just free
		//in case some other data type other than a primitive is
		//in PQueue
		pq->elements->releaseFunction(toFree);
	}
}
void add(PQueue* pq, Object obj){
	if(pq->mode == PQMODE_STACK){
		push(pq, obj);
		return;
	}
	if(pq->mode == PQMODE_QUEUE){
		enqueue(pq, obj);
		return;
	}
	assert(pq->priorityExtractor);
	//DList does not have an insert in order, so need to write it here
	//elements should go following prev <= current < next
	//this achieves that current is FIFO in the event that duplicate priorities are found
	DNode* current = pq->elements->head;
	if(!current){
		appendDList(pq->elements, obj);
		return;
	}
	uint count = 0;
	Int objHolder = allocInt();
	Int currentHolder = allocInt();
	pq->priorityExtractor(obj, objHolder);
	while(current && current->next){
		pq->priorityExtractor(current->data, currentHolder);
		if(*objHolder < *currentHolder){
			break;
		}
		current = current->next;
		++count;
	}

	pq->priorityExtractor(current->data, currentHolder);
	DNode* node = (DNode*)calloc(1, sizeof(DNode));
	node->data = calloc(1, pq->elements->elementSize);
	memcpy(node->data, obj, pq->elements->elementSize);
	node->next = NULL;
	node->prev = NULL;
	if(*objHolder < *currentHolder){
		//elements goes on the left of current, need to have a special case for head
		node->next = current;
		node->prev = current->prev;

		if(current == pq->elements->head){
			pq->elements->head = node;
		}
		else{
			current->prev->next = node;
		}
		current->prev = node;
	}
	else{
		//elements goes on the right of current, need to have a special case for tail
		node->prev = current;
		node->next = current->next;

		if(current == pq->elements->tail){
			pq->elements->tail = node;
		}
		else{
			current->next->prev = node;
		}
		current->next = node;
	}
	++pq->elements->size;
	releasePrimitive(objHolder);
	releasePrimitive(currentHolder);
		return;
	/*}
	if(current == pq->elements->head){
		insertDList(pq->elements, obj);
		return;
	}

	//current is somewhere between the list
	DNode* node = (DNode*)malloc(sizeof(DNode));
	node->next = current;
	node->prev = current->prev;
	current->prev->next = node;
	current->prev = node;
	++pq->elements->size;*/
}
bool isEmptyPQueue(PQueue* pq){
	return pq->elements->size == 0;
}

Object top(PQueue* pq){
	assert(pq->mode == PQMODE_STACK);
	return peekMin(pq);
}
void pop(PQueue* pq){
	assert(pq->mode == PQMODE_STACK);
	globalPriority = isEmptyPQueue(pq) ? 0 : globalPriority + 1;
	removeMin(pq);
}
void push(PQueue* pq, Object obj){
	assert(pq->mode == PQMODE_STACK);
	Int emptyHolder = allocInt();
	pq->priorityExtractor(obj, emptyHolder);
	insertDList(pq->elements, obj);
	releasePrimitive(emptyHolder);
}
Object front(PQueue* pq){
	assert(pq->mode == PQMODE_QUEUE);
	return peekMin(pq);
}
void dequeue(PQueue* pq){
	assert(pq->mode == PQMODE_QUEUE);
	globalPriority = isEmptyPQueue(pq) ? 0 : globalPriority - 1;
	removeMin(pq);
}
void enqueue(PQueue* pq, Object obj){
	assert(pq->mode == PQMODE_QUEUE);
	Int emptyHolder = allocInt();
	pq->priorityExtractor(obj, emptyHolder);
	appendDList(pq->elements, obj);
	releasePrimitive(emptyHolder);
}

void setComparatorPQueue(PQueue* pq, void (*extractor)(Object, Object)){
	pq->priorityExtractor = extractor;
}
void setPrinterPQueue(PQueue* pq, void (*printer)(FILE*, Object)){
	pq->elements->printerFunction = printer;
}
void setReleasePQueue(PQueue* pq, void (*releaser)(Object)){
	pq->elements->releaseFunction = releaser;
}

void printPQueue(FILE* out, PQueue* pq){
	printDList(out, pq->elements);
}
void debugPQueue(FILE* out, PQueue* pq){
	debugDList(out, pq->elements);
}

void sortIntPriority(Object obj, Object copy){
	if(!copy){
		copy = malloc(sizeof(int));
	}
	memcpy(copy, obj, sizeof(int));
}

void stackPriority(Object obj, Object copy){
	if(!copy){
		copy = malloc(sizeof(int));
	}
	memcpy(copy, &globalPriority, sizeof(int));
	--globalPriority;
}
void queuePriority(Object obj, Object copy){
	if(!copy){
		copy = malloc(sizeof(int));
	}
	memcpy(copy, &globalPriority, sizeof(int));
	++globalPriority;
}

///////////////TEST FUNCTIONS
void testPQueue(int argc, char** argv){
	PQueue* pq = allocPQueue(sizeof(int), PQMODE_PQUEUE);
	setComparatorPQueue(pq, &sortIntPriority);
	setPrinterPQueue(pq, &printInt);
	setReleasePQueue(pq, &releasePrimitive);
	printPQueue(stdout, pq);
	printf("\n");
	Int x = allocIntWith(5);
	add(pq, x);
	printPQueue(stdout, pq);
	printf("\n");
	*x = 3;
	add(pq, x);
	printPQueue(stdout, pq);
	printf("\n");
	*x = 7;
	add(pq, x);
	printPQueue(stdout, pq);
	printf("\n");
	*x = 1;
	add(pq, x);
	printPQueue(stdout, pq);
	printf("\n");
	*x = 5;
	add(pq, x);
	printPQueue(stdout, pq);
	printf("\n");
	*x = 7;
	add(pq, x);
	printPQueue(stdout, pq);
	printf("\n");
	releasePrimitive(x);
}
