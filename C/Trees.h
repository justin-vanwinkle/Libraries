/*
 * Trees.h
 *
 *  Created on: Jul 3, 2014
 *      Author: lugoh008
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>
#include "DArray.h"
#include "Objects.h"

#ifndef TREES_H_
#define TREES_H_

//this counter is only for demo purposes
//to show the BST search speed
static uint searchCounter = 0;


//Iterators that define how to traverse a tree (in what order)
typedef enum _TreeIterator{
	PREORDER,
	INORDER,
	POSTORDER
} TreeIterator;


//The basic component of a tree, its internal nodes have two children (left and right)
typedef struct _BTNode{
	Object data;			//generic data container
	struct _BTNode* left;	//left child
	struct _BTNode* right;	//right child
} BTNode;

typedef struct BinarySearchTree {
	BTNode* root;			//the root of the tree
	uint elementSize;		//the size of all elements inside each BTNode data field

	void (*releaser)(Object);			//a function used to release the Object data inside the individual nodes
	int (*comparator)(Object, Object);	//key function to compare objects and decide weather info should go to the left or right
	void (*printer)(FILE*, Object);		//print function to specify how to print the internal Object data inside BTNodes
} BSTree;

/*
 * Function that allocates a Binary Search Tree by specifying
 * 1) elementSize
 * 2) release function (as a function pointer)
 * 3) compare function (as a function pointer)
 */
BSTree* allocBST(uint, void (*rel)(Object), int (*comp)(Object, Object));

/*
 * Pending: function to release a tree
 */
void releaseBST(BSTree*);

/*
 * function to set the internal printer that handles Object data printing.
 * Any function that is void and received the destination stream (FILE*) and data to be print (Objet)
 * is valid here
 */
void setPrinterBST(BSTree*, void (*print)(FILE*, Object));

/*
 * Adds an element to the tree keeping the in order storage
 */
void addBST(BSTree*, Object);

/*
 * Removes an element (the first appearance) from the BST and
 * Returns true if the element was removed
 * It returns false if the element was never found
 */
bool removeBST(BSTree*, Object);

/*
 * Removes the min element of a BST
 */
void removeMinBST(BSTree*);

/*
 * Removes the max element of a BST
 */
void removeMaxBST(BSTree*);

/**
 * Gets the ancestor of a BTNode inside the tree
 * This function does not return a copy so be careful not to release it
 */
BTNode* ancestorBST(BSTree*, BTNode*);

/**
 * Computes the height of the tree, where leaves are at height 0
 * Every level before that increases its height by 1, until the root is reached
 * The height of a tree is its distance from the root to its farthest leaf
 */
uint heightBST(BSTree*);

/**
 * Finds the minimum element in a BST, located at the farthest left node
 */
Object minBST(BSTree*);


/**
 * Finds the maximum element in a BST, located at the farthest right node
 */
Object maxBST(BSTree*);


/*
 * A function that tests for the presence of a value inside a tree
 */
bool containsBST(BSTree*, Object);

/**
 * Is empty checks if the tree has no more nodes
 */
bool isEmptyBST(BSTree*);

/*
 * Prints a Binary Search Tree into a specified stream
 * FILE* - the stream of where to print data, could be simply stdout (console)
 * BSTree* - the tree to be printed
 */
void printBST(FILE*, BSTree*);

/**
 * Lab 4 functions ...
 */

ArrayList*  BST2Array(BSTree*);

#endif /* TREES_H_ */
