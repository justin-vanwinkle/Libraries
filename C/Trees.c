/*
 * Trees.c
 *
 *  Created on: Jul 3, 2014
 *      Author: lugoh008
 */

#include "Trees.h"

//auxiliary function prototypes for .h functions
void addBST_aux(BSTree*, Object, BTNode*);
void BST2Array_aux(BSTree*, ArrayList*, BTNode*);
bool removeBST_aux(BSTree*, Object, BTNode*);
void removeMinBST_aux(BSTree*, BTNode*);
void removeMaxBST_aux(BSTree*, BTNode*);
//min and max aux functions are not used by the tree function, but they help during remove
Object minBST_aux(BSTree*, BTNode*);
Object maxBST_aux(BSTree*, BTNode*);
uint heightBST_aux(BTNode*);
bool containsBST_aux(BSTree*, Object, BTNode*);
void printBST_aux(FILE*, BSTree*, BTNode*);

BSTree* allocBST(uint esize, void (*rel)(Object), int (*comp)(Object, Object)){
	//using calloc(1, sizeof) instead of malloc because calloc sets everything to 0
	//this causes root and its left and right to be NULL by default
	BSTree* tree = calloc(1, sizeof(BSTree));
	//The following two lines are not needed because of calloc (inits everything to 0)
	//tree->root = NULL;
	//tree->printer = NULL;
	tree->elementSize = esize;
	//when releasing call tree->releaser(node->data)
	tree->releaser = rel;
	//when comparing call tree->comparator(x, y)
	tree->comparator = comp;
	return tree;
}
void releaseBST(BSTree* tree){
	//TODO: NEED DIFFERENT ITERATION TO RELEASE TREE
	if(tree){
		if(tree->root){

		}
		free(tree);
	}
}
void setPrinterBST(BSTree* tree, void (*print)(FILE*, Object)){
	//later to call the print invoke as tree->printer(obj)
	//see printBST bellow
	tree->printer = print;
}

void addBST(BSTree* tree, Object obj){
	if(tree){
		//can only add if we have a non-NULL tree
		if(tree->root){
			//if I have a root then I need to traverse the tree to find the spot for new data to come in
			addBST_aux(tree, obj, tree->root);
			return;
		}
		//tree is empty
		//place data in tree->root
		tree->root = calloc(1, sizeof(BTNode));
		tree->root->data = calloc(1, tree->elementSize);
		memcpy(tree->root->data, obj, tree->elementSize);
	}
}
void addBST_aux(BSTree* tree, Object obj, BTNode* node){
	if(tree->comparator(obj, node->data) < 0){
		//obj is smaller than node->data so it must go at the left of the node
		if(!node->left){
			//create new node here
			node->left = (BTNode*)calloc(1, sizeof(BTNode));
			node->left->data = calloc(1, tree->elementSize);
			memcpy(node->left->data, obj, tree->elementSize);
			return;
		}
		//else call recursion
		addBST_aux(tree, obj, node->left);
	}
	else{
		//obj must go at the right of node
		if(!node->right){
			//create new node here
			node->right = (BTNode*)calloc(1, sizeof(BTNode));
			node->right->data = calloc(1, tree->elementSize);
			memcpy(node->right->data, obj, tree->elementSize);
			return;
		}
		//else call recursion
		addBST_aux(tree, obj, node->right);
	}
}

bool removeBST(BSTree* tree, Object obj){
	if(!tree || !tree->root){
		return false;
	}
	return removeBST_aux(tree, obj, tree->root);
}

bool removeBST_aux(BSTree* tree, Object obj, BTNode* node){
	if(tree->comparator(obj, node->data) == 0){
		//element was found in the tree
		//but the question is how do we remove it?
		//leaves have no problem, because they are located at the end
		//and have no children

		//worst case scenario is what if want to remove the root of the tree?
		//we need to have an element to take its place
		//but this is only needed if the root has at least one child
		Object replacement = NULL;	//NULL is not memory allocation, therefore memory leak
		if(node->left){
			//we can grab the max element from the left (its a leaf)
			//remove it from the tree and place it on the position where the
			//element we want to remove is
			replacement = maxBST_aux(tree, node->left);
			//remove duplicate
			removeMaxBST_aux(tree, node->left);
			//copy replacement to node
			memcpy(node->data, replacement, tree->elementSize);
		}
		else if(node->right){
			//we can grab the min element from the right (its a leaf)
			//remove it from the tree and place it on the position where the
			//element we want to remove is
			replacement = minBST_aux(tree, node->right);
			//remove duplicate
			removeMinBST_aux(tree, node->right);
			//copy replacement to node
			memcpy(node->data, replacement, tree->elementSize);
		}
		return true;
	}
	//try to find it on left or right child
	bool found = false;
	if(tree->comparator(obj, node->data) < 0){
		//should search at the left
		if(node->left){
			//bitwise or |, performs the operation of aggregating booleans
			//as long as one remove returns true, the whole recursion returns true
			found =  found | removeBST_aux(tree, obj, node->left);
		}
	}
	else{
		if(node->right){
			//bitwise or |, performs the operation of aggregating booleans
			//as long as one remove returns true, the whole recursion returns true
			found =  found | removeBST_aux(tree, obj, node->right);
		}
		//should search at the right
	}
	return found;
}

void removeMinBST(BSTree* tree){
	//root has no parent
	BTNode* parent = NULL;
	BTNode* node = tree->root;
	while(node->left){
		parent = node;
		node = node->left;
	}
	//node is at the minimum, if this node is a leaf then everything is good to go
	//if it is not a leaf we need to reconnect the child to the parent of the node that is being removed
	//luckily, there is only one child at most (at the right)
	if(node->right){
		//we were coming from the left
		if(parent){
			parent->left = node->right;
		}
		else{
			//removing the root of the tree, need a new one
			tree->root = node->right;
		}
	}
	else{
		//disconnect leaf
		if(parent){
			parent->left = NULL;
		}
		else{
			//removing the last leaf (the root)
			tree->root = NULL;
		}
	}
	node->right = NULL;
	//node is disconnected, proceed to free data
	tree->releaser(node->data);
	//free the node
	free(node);
}
void removeMinBST_aux(BSTree* tree, BTNode* node){
	//to make this work as the non-aux functions we need to reach the node first
	//this is very easy because BST is ordered
	//this is only needed to get the parent of the node in case reconnection is needed

	BTNode* parent = ancestorBST(tree, node);

	//once iterator == null we have parent as the ancestor of node
	//proceed as removeMinBST

	while(node->left){
		parent = node;
		node = node->left;
	}
	//node is at the minimum, if this node is a leaf then everything is good to go
	//if it is not a leaf we need to reconnect the child to the parent of the node that is being removed
	//luckily, there is only one child at most (at the right)
	if(node->right){
		//we were coming from the left
		if(parent){
			parent->right = node->right;
		}
		else{
			//removing the root of the tree, need a new one
			tree->root = node->right;
		}
	}
	else{
		//disconnect leaf
		if(parent){
			parent->right = NULL;
		}
		else{
			//removing the last leaf (the root)
			tree->root = NULL;
		}
	}
	node->right = NULL;
	//node is disconnected, proceed to free data
	tree->releaser(node->data);
	//free the node
	free(node);
}
void removeMaxBST(BSTree* tree){
	//root has no parent
	BTNode* parent = NULL;
	BTNode* node = tree->root;
	while(node->right){
		parent = node;
		node = node->right;
	}
	//node is at the maximum, if this node is a leaf then everything is good to go
	//if it is not a leaf we need to reconnect the child to the parent of the node that is being removed
	//luckily, there is only one child at most (at the left)
	if(node->left){
		//we were coming from the right
		if(parent){
			parent->right = node->left;
		}
		else{
			//removing the root of the tree, need a new one
			tree->root = node->left;
		}
	}
	else{
		//disconnect leaf
		if(parent){
			parent->right = NULL;
		}
		else{
			//removing the last leaf (the root)
			tree->root = NULL;
		}
	}
	node->left = NULL;
	//node is disconnected, proceed to free data
	tree->releaser(node->data);
	//free the node
	free(node);
}
void removeMaxBST_aux(BSTree* tree, BTNode* node){
	//to make this work as the non-aux functions we need to reach the node first
	//this is very easy because BST is ordered
	//this is only needed to get the parent of the node in case reconnection is needed

	BTNode* parent = ancestorBST(tree, node);

	//once iterator == null we have parent as the ancestor of node
	//proceed as removeMinBST
	while(node->right){
		parent = node;
		node = node->right;
	}
	//node is at the maximum, if this node is a leaf then everything is good to go
	//if it is not a leaf we need to reconnect the child to the parent of the node that is being removed
	//luckily, there is only one child at most (at the left)
	if(node->left){
		//logic is different than when the non-aux function
		//because this is used to remove the duplicate created
		//when replacing the parent value with the left subtree max
		if(parent){
			parent->left = node->left;
		}
		else{
			//removing the root of the tree, need a new one
			tree->root = node->left;
		}
	}
	else{
		//disconnect leaf
		if(parent){
			parent->left = NULL;
		}
		else{
			//removing the last leaf (the root)
			tree->root = NULL;
		}
	}
	node->left = NULL;
	//node is disconnected, proceed to free data
	tree->releaser(node->data);
	//free the node
	free(node);
}

//min and max functions dont need auxiliary functions because we can write them
//without recursion
Object minBST(BSTree* tree){
	if(!tree || !tree->root){
		return NULL;
	}
	BTNode* iterator = tree->root;
	while(iterator->left){
		iterator = iterator->left;
	}
	//once no more left children are found we are at the farthest left node (min)
	Object ret = calloc(1, tree->elementSize);
	memcpy(ret, iterator->data, tree->elementSize);
	return ret;
}

Object maxBST(BSTree* tree){
	if(!tree || !tree->root){
		return NULL;
	}
	BTNode* iterator = tree->root;
	while(iterator->right){
		iterator = iterator->right;
	}
	//once no more right children are found we are at the farthest right node (min)
	Object ret = calloc(1, tree->elementSize);
	memcpy(ret, iterator->data, tree->elementSize);
	return ret;
}

//helper for remove
Object minBST_aux(BSTree* tree, BTNode* node){
	if(!node){
		return NULL;
	}
	BTNode* iterator = node;
	while(iterator->left){
		iterator = iterator->left;
	}
	//once no more left children are found we are at the farthest left node (min)
	Object ret = calloc(1, tree->elementSize);
	memcpy(ret, iterator->data, tree->elementSize);
	return ret;
}

Object maxBST_aux(BSTree* tree, BTNode* node){
	if(!node){
		return NULL;
	}
	BTNode* iterator = node;
	while(iterator->right){
		iterator = iterator->right;
	}
	//once no more left children are found we are at the farthest left node (min)
	Object ret = calloc(1, tree->elementSize);
	memcpy(ret, iterator->data, tree->elementSize);
	return ret;
}

BTNode* ancestorBST(BSTree* tree, BTNode* node){
	BTNode* parent = NULL;
	BTNode* iterator = tree->root;
	//this code assumes that node is in tree
	while(iterator != node){
		parent = iterator;
		if(tree->comparator(node->data, iterator->data) < 0){
			//node must be on the left side of iterator
			iterator = iterator->left;
		}
		else{
			//otherwise must be at the right because we assume node is in tree
			iterator = iterator->right;
		}
	}
	return parent;
}

uint heightBST(BSTree* tree){
	assert(tree);
	if(!tree->root){
		return 0;
	}
	//just call the auxiliary function as we know that the tree at least has a root
	return heightBST_aux(tree->root);
}

uint heightBST_aux(BTNode* node){
	if(!node->left && !node->right){
		//leaves have a height of 0
		return 0;
	}
	//height_BST_aux expects a non-NULL node, so only call it when node is not NULL
	//otherwise 0 is returned automatically
	uint lheight = node->left ? heightBST_aux(node->left) : 0;
	uint rheight = node->right ? heightBST_aux(node->right) : 0;
	//the height of the tree is its max height out of the left and right height
	uint height = lheight > rheight ? lheight : rheight;
	//1 + height causes the increase in height on every level
	return 1 + height;
}

bool containsBST(BSTree* tree, Object obj){
	//search counter is just for demo purposes
	//I want to demonstrate you how fast a BST can be during search
	searchCounter = 0;
	if(tree && tree->root){
		//an element can be within the tree only if the tree is not empty
		return containsBST_aux(tree, obj, tree->root);
	}
	return false;
}
bool containsBST_aux(BSTree* tree, Object obj, BTNode* node){
	//is call the counter is increased so that we can see how fast the search
	//was carried out
	++searchCounter;
	if(!node){
		//if I dont have a node then I dont have data
		return false;
	}
	if(tree->comparator(obj, node->data) == 0){
		//found data since comparator gave 0
		return true;
	}
	if(tree->comparator(obj, node->data) < 0){
		//data is smaller than the current node being evaluated
		//goto left to continue.
		return containsBST_aux(tree, obj, node->left);
	}
	//goto right as data should be there (obj > node->data)
	return containsBST_aux(tree, obj, node->right);
}

bool isEmptyBST(BSTree* tree){
	return !tree || !tree->root;
}

//TODO: add different iterators, for now its just INORDER
void printBST(FILE* out, BSTree* tree){
	fprintf(out, "{");
	if(tree && tree->root){
		printBST_aux(out, tree, tree->root);
	}
	fprintf(out, "}");
}
void printBST_aux(FILE* out, BSTree* tree, BTNode* node){
	if(!node){
		return;
	}
	printBST_aux(out, tree, node->left);
	printBST_aux(out, tree, node->right);
	tree->printer(out, node->data);
	fprintf(out, ", ");
}

int testSimpleBST(int argc, char** argv){
	srand(100);
	BSTree* tree = allocBST(sizeof(int), &releasePrimitive, &compareInt);
	setPrinterBST(tree, &printInt);
	printBST(stdout, tree);
	printf("\n");
	Int x = allocInt();
	int i = 0;
	for(i = 10; i >= 0; --i){
		*x = rand() % 100;
		addBST(tree, x);
	}
	printBST(stdout, tree);
	printf("\n");
	*x = 84;
	if(containsBST(tree, x)){
		printf("%d is contained in the tree\n", *x);
		printf("Found in %d steps\n",searchCounter);
		removeBST(tree, x);
		printBST(stdout, tree);
		printf("\n");
	}
	else{
		printf("%d is not contained in the tree\n", *x);
		printf("Not found in %d steps\n",searchCounter);
	}
	printf("Height of the tree %d\n", heightBST(tree));
	if(isEmptyBST(tree)){
		printf("The tree is empty\n");
	}
	//removeMaxBST(tree);
	//removeMinBST(tree);
	//printBST(stdout, tree);
	//printf("\n");

	releasePrimitive(x);
	//still not releasing the tree, work pending
	return 0;
}

int testBST(int argc, char** argv){
	srand(100);
	BSTree* tree = allocBST(sizeof(int), &releasePrimitive, &compareInt);
	setPrinterBST(tree, &printInt);
	printBST(stdout, tree);
	printf("\n");
	Int x = allocInt();
	int i = 0;
	for(i = 20; i >= 0; --i){
		*x = rand() % 100;
		addBST(tree, x);
	}
	printBST(stdout, tree);
	printf("\n");
	*x = 97;
	if(containsBST(tree, x)){
		printf("%d is contained in the tree\n", *x);
		printf("Found in %d steps\n",searchCounter);
		removeBST(tree, x);
		printBST(stdout, tree);
		printf("\n");
	}
	else{
		printf("%d is not contained in the tree\n", *x);
		printf("Not found in %d steps\n",searchCounter);
	}
	printf("Height of the tree %d\n", heightBST(tree));
	//removeMaxBST(tree);
	//removeMinBST(tree);
	//printBST(stdout, tree);
	//printf("\n");

	releasePrimitive(x);
	//still not releasing the tree, work pending
	return 0;
}

int testLabBST(int argc, char** argv){
	BSTree* tree = allocBST(sizeof(int), &releasePrimitive, &compareInt);
	setPrinterBST(tree, &printInt);
	printBST(stdout, tree);
	printf("\n");
	int i = 0;
	int elements[] = {7, 5, 15, 5, 11, 83};
	for(i = 0; i < sizeof(elements) / sizeof(int); ++i){
		addBST(tree, &elements[i]);
	}
	printBST(stdout, tree);
	printf("\nHEIGHT = %d\n", heightBST(tree));

	ArrayList* array = BST2Array(tree);
	setReleaseDArray(array, tree->releaser);
	setPrinterDArray(array, tree->printer);
	printDArray(stdout, array);
	releaseDArray(array);

	//still not releasing the tree, work pending
	return 0;
}


