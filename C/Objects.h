/*
 * Objects.h
 *
 *  Created on: Jun 25, 2014
 *      Author: lugoh008
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//aliases for unsigned primitives
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef uchar Byte;
typedef ushort Word;

//aliases for primitives pointers (can be use in place of Object)
typedef bool* Bool;
typedef uchar* UChar;
typedef char* Char;
typedef ushort* UShort;
typedef short* Short;
typedef uint* UInt;
typedef int* Int;
typedef float* Float;
typedef ulong* ULong;
typedef long* Long;
typedef double* Double;
typedef char* String;

//aliases for generic pointers
typedef void* Object;
typedef Object* GenericArray;



//alloc functions for primitives
Object allocBool();
Object allocUChar();
Object allocChar();
Object allocUShort();
Object allocShort();
Object allocInt();
Object allocUInt();
Object allocFloat();
Object allocULong();
Object allocLong();
Object allocDouble();
Object allocString(uint);

//alloc functions for primitives with initial value
Object allocBoolWith(bool);
Object allocUCharWith(uchar);
Object allocCharWith(char);
Object allocUShortWith(ushort);
Object allocShortWith(short);
Object allocIntWith(int);
Object allocUIntWith(uint);
Object allocFloatWith(float);
Object allocULongWith(ulong);
Object allocLongWith(long);
Object allocDoubleWith(double);
Object allocStringWith(String);

//release functions for primitives
void releasePrimitive(Object);

//copy functions (from, to)
void copyBool(Bool, Bool);
void copyUChar(UChar, UChar);
void copyChar(Char, Char);
void copyUShort(UShort, UShort);
void copyShort(Short, Short);
void copyInt(Int, Int);
void copyUInt(UInt, UInt);
void copyFloat(Float, Float);
void copyULong(ULong, ULong);
void copyLong(Long, Long);
void copyDouble(Double, Double);
void copyString(String, String);

//compare functions (this, other)
/**
 * return 0 if equal
 * 		  1 if this > other
 * 		  -1 if this < other
 */
int compareBool(Object, Object);
int compareUChar(Object, Object);
int compareChar(Object, Object);
int compareUShort(Object, Object);
int compareShort(Object, Object);
int compareInt(Object, Object);
int compareUInt(Object, Object);
int compareFloat(Object, Object);
int compareULong(Object, Object);
int compareLong(Object, Object);
int compareDouble(Object, Object);
int compareString(Object, Object);


//printer functions
void printBool(FILE* , Object);
void printUChar(FILE*, Object);
void printChar(FILE*, Object);
void printUShort(FILE*, Object);
void printShort(FILE*, Object);
void printInt(FILE*, Object);
void printUInt(FILE*, Object);
void printFloat(FILE*, Object);
void printULong(FILE*, Object);
void printLong(FILE*, Object);
void printDouble(FILE*, Object);
void printString(FILE*, Object);
void printObject(FILE*, Object);

#endif /* OBJECTS_H_ */
