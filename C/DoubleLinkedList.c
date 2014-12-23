/*
 * DoubleLinkedList.c
 *
 *  Created on: Jun 17, 2014
 *      Author: lugoh008
 */

#include "DoubleLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DoubleLinkedList* allocDList(uint elementSize, void (*releaser)(Object), void (*printer)(FILE*, Object)){
	DoubleLinkedList* list = (DoubleLinkedList*)calloc(1, sizeof(DoubleLinkedList));
	list->size = 0;
	list->elementSize = elementSize;
	list->releaseFunction = releaser;
	list->printerFunction = printer;
	return list;
}

//This function is not access directly because it is not part of the DoubleLinkedList.h
void releaseDNode(DNode* node, void (*releaseData)(Object)){
	if(node){
		if(node->data){
			//ideally this should be a release function
			releaseData(node->data);
		}
		if(node->next){
			releaseDNode(node->next, releaseData);
		}
		free(node);
	}
}

void releaseDList(DoubleLinkedList* list){
	if(list){
		if(list->head){
			releaseDNode(list->head, list->releaseFunction);
		}
		free(list);
	}
}

void insertDListElementAt(DoubleLinkedList* list, Object newElement, uint position){
	if(!list->head){
		appendDList(list, newElement);
		return;
	}
	uint fromStart = position;
	uint fromEnd = list->size - fromStart - 1;
	if(fromStart == 0){
		insertDList(list, newElement);
		return;
	}
	if(fromEnd == 0 || fromEnd == -1){
		appendDList(list, newElement);
		return;
	}
	DNode* iterator;
	uint index = 0;
	if(fromStart <= fromEnd){
		//search for the position forward
		iterator = list->head;
		for(index = 0; index < fromStart; ++index){
			iterator = iterator->next;
		}
	}
	else{
		//search for the position backward
		iterator = list->tail;
		for(index = 0; index < fromEnd; ++index){
			iterator = iterator->prev;
		}
	}
	DNode* newNode = (DNode*)calloc(1, sizeof(DNode));
	newNode->data = calloc(1, list->elementSize);
	memcpy(newNode->data, newElement, list->elementSize);
	newNode->prev = iterator->prev;
	newNode->next = iterator;
	iterator->prev->next = newNode;
	iterator->prev = newNode;
	++list->size;
}

void setElementAtDList(DoubleLinkedList* list, uint position, Object newValue){
	if(!list->head){
		return;
	}
	uint fromStart = position;
	uint fromEnd = list->size - fromStart - 1;
	DNode* iterator;
	uint index = 0;
	if(fromStart <= fromEnd){
		//search for the position forward
		iterator = list->head;
		for(index = 0; index < fromStart; ++index){
			iterator = iterator->next;
		}
	}
	else{
		//search for the position backward
		iterator = list->tail;
		for(index = 0; index < fromEnd; ++index){
			iterator = iterator->prev;
		}
	}
	memcpy(iterator->data, newValue, list->elementSize);
}
Object getElementAtDList(DoubleLinkedList* list, uint position){
	if(!list->head){
		return NULL;
	}
	uint fromStart = position;
	uint fromEnd = list->size - fromStart - 1;
	DNode* iterator;
	uint index = 0;
	if(fromStart <= fromEnd){
		//search for the position forward
		iterator = list->head;
		for(index = 0; index < fromStart; ++index){
			iterator = iterator->next;
		}
	}
	else{
		//search for the position backward
		iterator = list->tail;
		for(index = 0; index < fromEnd; ++index){
			iterator = iterator->prev;
		}
	}
	Object data = malloc(list->elementSize);
	memcpy(data, iterator->data, list->elementSize);
	return data;
}

void appendDList(DoubleLinkedList* list, Object newElement){
	if(!list->head){
		//list is empty then tail and head will be the newElement
		list->tail = (DNode*)calloc(1, sizeof(DNode));
		list->tail->data = malloc(list->elementSize);
		memcpy(list->tail->data, newElement, list->elementSize);
		list->head = list->tail;
	}
	else{
		DNode* newNode = (DNode*)calloc(1, sizeof(DNode));
		newNode->data = malloc(list->elementSize);
		memcpy(newNode->data, newElement, list->elementSize);
		list->tail->next = newNode;
		newNode->prev = list->tail;
		list->tail = newNode;
	}
	++list->size;
}

void insertDList(DoubleLinkedList* list, Object newElement){
	if(!list->head){
		appendDList(list, newElement);
		return;
	}
	DNode* newNode = (DNode*)calloc(1, sizeof(DNode));
	newNode->data = malloc(list->elementSize);
	memcpy(newNode->data, newElement, list->elementSize);
	list->head->prev = newNode;
	newNode->next = list->head;
	list->head = newNode;
	++list->size;
}

DoubleLinkedList* reverseList(DoubleLinkedList* list){
	DoubleLinkedList* rcopy = allocDList(list->elementSize, list->releaseFunction, list->printerFunction);
	DNode* iterator = list->tail;
	while(iterator){
		appendDList(rcopy, iterator->data);
		iterator = iterator->prev;
	}
	return rcopy;
}

void breakEvenDList(DoubleLinkedList* list, DoubleLinkedList** lower, DoubleLinkedList** upper){
	if(lower){
		releaseDList(*lower);
		//free(lower);
	}
	if(upper){
		releaseDList(*upper);
		//free(upper);
	}
	//lower = (DoubleLinkedList**)malloc(sizeof(DoubleLinkedList*));
	//upper = (DoubleLinkedList**)malloc(sizeof(DoubleLinkedList*));
	//*lower = NULL;
	//*upper = NULL;
	*lower = allocDList(list->elementSize, list->releaseFunction, list->printerFunction);
	*upper = allocDList(list->elementSize, list->releaseFunction, list->printerFunction);
	DNode* start = list->head;
	DNode* end = list->tail;
	uint halfSize = list->size / 2;
	uint count = 0;

	while(start && end && count < halfSize){
		appendDList(*lower, start->data);
		insertDList(*upper, end->data);
		start = start->next;
		end = end->prev;
		++count;
	}
	if(start == end){
		//when odd number of elements upper is greater than lower
		//(e.g. 5/2 = 2 and 3 surplus)
		insertDList(*upper, end->data);
	}
}

Object removeDList(DoubleLinkedList* list, uint position){
	if(!list->head){
		return NULL;
	}
	uint fromStart = position;
	uint fromEnd = list->size - fromStart - 1;
	DNode* iterator;
	uint index = 0;
	if(fromStart <= fromEnd){
		//search for the position forward
		iterator = list->head;
		for(index = 0; index < fromStart; ++index){
			iterator = iterator->next;
		}
	}
	else{
		//search for the position backward
		iterator = list->tail;
		for(index = 0; index < fromEnd; ++index){
			iterator = iterator->prev;
		}
	}
	//iterator is the node we want to remove
	if(iterator != list->head){
		iterator->prev->next = iterator->next;
	}
	else{
		list->head = iterator->next;
		if(list->head)
			list->head->prev = NULL;
	}
	if(iterator != list->tail){
		iterator->next->prev = iterator->prev;
	}
	else{
		list->tail = iterator->prev;
		if(list->tail)
			list->tail->next = NULL;
	}
	iterator->prev = NULL;
	iterator->next = NULL;
	//returning the data
	Object retData = malloc(list->elementSize);
	memcpy(retData, iterator->data, list->elementSize);
	//list->releaseFunction(iterator->data);
	releaseDNode(iterator, list->releaseFunction);
	--list->size;
	return retData;
}

void printDList(FILE* out, DoubleLinkedList* list){
	if(!out){
		out = stdout;
	}
	if(!list){
		fprintf(out, "NULL");
		return;
	}
	fprintf(out, "{");
	DNode* iterator = list->head;
	while(iterator){
		list->printerFunction(out, iterator->data);
		iterator = iterator->next;
		if(iterator){
			fprintf(out, ", ");
		}
	}
	//free(iterator);
	fprintf(out, "}");
}

void debugDList(FILE* out, DoubleLinkedList* list){
	if(!out){
		out = stdout;
	}
	if(!list){
		fprintf(out, "NULL");
		return;
	}
	DNode* iter = list->head;
	while(iter){
		fprintf(out, "%p (%p = ", iter, iter->data);
		list->printerFunction(out, iter->data);
		fprintf(out, ")\t");
		iter = iter->next;
	}
	printf("\n");
	iter = list->tail;
	while(iter){
		fprintf(out, "%p (%p = ", iter, iter->data);
		list->printerFunction(out, iter->data);
		fprintf(out, ")\t");
		iter = iter->prev;
	}
	printf("\n\n");
}


int testDList(int argc, char** argv){
	DoubleLinkedList* list = allocDList(sizeof(char), &releasePrimitive, &printChar);
	char test = 'A';
	printf("TESTING APPEND ...\n\n");
	appendDList(list, &test);
	debugDList(stdout, list);
	printf("\n");
	printDList(stdout, list);
	printf("\n");
	test += 1;
	appendDList(list, &test);
	debugDList(stdout, list);
	printf("\n");
	printDList(stdout, list);
	printf("\n");
	test += 1;
	appendDList(list, &test);
	debugDList(stdout, list);
	printf("\n");
	printDList(stdout, list);
	printf("\n");
	test += 1;
	printf("TESTING INSERT ...\n\n");
	insertDList(list, &test);
	debugDList(stdout, list);
	printf("\n");
	printDList(stdout, list);
	printf("\n");
	test += 1;
	printf("TESTING INSERT AT 2 then 1 ...\n\n");
	insertDListElementAt(list, &test, 2);
	test += 1;
	insertDListElementAt(list, &test, 1);
	debugDList(stdout, list);
	printf("\n");
	printDList(stdout, list);
	printf("\n");

	//half list
	printf("TESTING HALF ...\n\n");
	DoubleLinkedList* lower = NULL;
	DoubleLinkedList* upper = NULL;
	breakEvenDList(list, &lower, &upper);
	printf("LOWER ...\n");
	debugDList(stdout, lower);
	printf("\n");
	printDList(stdout, lower);
	printf("\n");
	printf("UPPER ...\n");
	debugDList(stdout, upper);
	printf("\n");
	printDList(stdout, upper);
	printf("\n");

	//reverse list
	printf("TESTING REVERSE ...\n\n");
	DoubleLinkedList* reverse = reverseList(list);
	debugDList(stdout, reverse);
	printf("\n");
	printDList(stdout, reverse);
	printf("\n");

	//testing remove
	printf("TESTING REMOVE ...\n\n");
	while(reverse->size){
		removeDList(reverse, 0);
		debugDList(stdout, reverse);
		printf("\n");
		printDList(stdout, reverse);
		printf("\n");
	}

	Char nchar = allocCharWith('Z');
	appendDList(reverse, nchar);
	releasePrimitive(nchar);
	debugDList(stdout, reverse);
	printf("\n");
	printDList(stdout, reverse);
	printf("\n");
	printChar(stdout, getElementAtDList(reverse, 0));
	printf("\n");
	nchar = allocCharWith('H');
	setElementAtDList(reverse, 0, nchar);
	releasePrimitive(nchar);
	printChar(stdout, getElementAtDList(reverse, 0));
	printf("\n");
	debugDList(stdout, reverse);
	printf("\n");
	printDList(stdout, reverse);
	printf("\n");


	printf("TESTING RANDOM ELEMENT REMOVAL ...\n\n");
	Object data = removeDList(list, 1);
	printf("REMOVED ");
	printChar(stdout, data);
	printf("\n");
	debugDList(stdout, list);
	printf("\n");
	printDList(stdout, list);
	printf("\n");

	releaseDList(reverse);
	releaseDList(lower);
	releaseDList(upper);
	releaseDList(list);
	return 0;
}
