/*
 * DoubleLinkedList.h
 *
 *  Created on: Jun 17, 2014
 *      Author: lugoh008
 */

#ifndef DOUBLELINKEDLIST_H_
#define DOUBLELINKEDLIST_H_

#include "Objects.h"

typedef struct _DNode {
	Object data;
	struct _DNode* next;
	struct _DNode* prev;
} DNode;

typedef struct _DoubleLinkedList{
	DNode* head;
	DNode* tail;
	uint elementSize;
	uint size;
	void (*releaseFunction)(Object);
	void (*printerFunction)(FILE*, Object);
} DoubleLinkedList;

DoubleLinkedList* allocDList(uint elementSize, void (*releaser)(Object), void (*printer)(FILE*, Object));
void releaseDList(DoubleLinkedList*);
void insertDListElementAt(DoubleLinkedList* list, Object newElement, uint position);
void appendDList(DoubleLinkedList* list, Object newElement);
void insertDList(DoubleLinkedList* list, Object newElement);
void setElementAtDList(DoubleLinkedList* list, uint position, Object newValue);
Object getElementAtDList(DoubleLinkedList* list, uint position);
DoubleLinkedList* reverseList(DoubleLinkedList* list);
void breakEvenDList(DoubleLinkedList* list, DoubleLinkedList** lower, DoubleLinkedList** upper);
Object removeDList(DoubleLinkedList* list, uint position);
void printDList(FILE*, DoubleLinkedList*);
void debugDList(FILE*, DoubleLinkedList*);

//tester function to be used in main, receives the same parameters as main
int testDList(int argc, char** argv);

#endif /* DOUBLELINKEDLIST_H_ */
