/*
 * Objects.c
 *
 *  Created on: Jun 25, 2014
 *      Author: lugoh008
 */


#include "Objects.h"


//alloc functions for primitives
Object allocBool(){
	return calloc(1, sizeof(bool));
}
Object allocUChar(){
	return calloc(1, sizeof(uchar));
}
Object allocChar(){
	return calloc(1, sizeof(char));
}
Object allocUShort(){
	return calloc(1, sizeof(ushort));
}
Object allocShort(){
	return calloc(1, sizeof(short));
}
Object allocInt(){
	return calloc(1, sizeof(int));
}
Object allocUInt(){
	return calloc(1, sizeof(uint));
}
Object allocFloat(){
	return calloc(1, sizeof(float));
}
Object allocULong(){
	return calloc(1, sizeof(ulong));
}
Object allocLong(){
	return calloc(1, sizeof(long));
}
Object allocDouble(){
	return calloc(1, sizeof(double));
}
Object allocString(uint slen){
	return calloc(slen, sizeof(char));
}

//alloc functions for primitives with initial value
Object allocBoolWith(bool e){
	Bool p = allocBool();
	*p = e;
	return p;
}
Object allocUCharWith(uchar e){
	UChar p = allocUChar();
	*p = e;
	return p;
}
Object allocCharWith(char e){
	Char p = allocChar();
	*p = e;
	return p;
}
Object allocUShortWith(ushort e){
	UShort p = allocUShort();
	*p = e;
	return p;
}
Object allocShortWith(short e){
	Short p = allocShort();
	*p = e;
	return p;
}
Object allocIntWith(int e){
	Int p = allocInt();
	*p = e;
	return p;
}
Object allocUIntWith(uint e){
	UInt p = allocUInt();
	*p = e;
	return p;
}
Object allocFloatWith(float e){
	Float p = allocFloat();
	*p = e;
	return p;
}
Object allocULongWith(ulong e){
	ULong p = allocULong();
	*p = e;
	return p;
}
Object allocLongWith(long e){
	Long p = allocLong();
	*p = e;
	return p;
}
Object allocDoubleWith(double e){
	Double p = allocDouble();
	*p = e;
	return p;
}
Object allocStringWith(String e){
	uint len = strlen(e);
	String p = allocString(len);
	memcpy(p, e, len);
	return p;
}

//release functions for primitives
void releasePrimitive(Object obj){
	free(obj);
}

//copy functions (from, to)
void copyBool(Bool from, Bool to){
	if(!from){
		return;
	}
	if(!to){
		to = allocBoolWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyUChar(UChar from, UChar to){
	if(!from){
		return;
	}
	if(!to){
		to = allocUCharWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyChar(Char from, Char to){
	if(!from){
		return;
	}
	if(!to){
		to = allocCharWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyUShort(UShort from, UShort to){
	if(!from){
		return;
	}
	if(!to){
		to = allocUShortWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyShort(Short from, Short to){
	if(!from){
		return;
	}
	if(!to){
		to = allocShortWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyInt(Int from, Int to){
	if(!from){
		return;
	}
	if(!to){
		to = allocIntWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyUInt(UInt from, UInt to){
	if(!from){
		return;
	}
	if(!to){
		to = allocUIntWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyFloat(Float from, Float to){
	if(!from){
		return;
	}
	if(!to){
		to = allocFloatWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyULong(ULong from, ULong to){
	if(!from){
		return;
	}
	if(!to){
		to = allocULongWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyLong(Long from, Long to){
	if(!from){
		return;
	}
	if(!to){
		to = allocLongWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyDouble(Double from, Double to){
	if(!from){
		return;
	}
	if(!to){
		to = allocDoubleWith(*from);
	}
	else{
		*to = *from;
	}
}
void copyString(String from, String to){
	if(!from){
		return;
	}
	if(!to){
		to = allocStringWith(from);
	}
	else{
		memcpy(to, from, strlen(from));
	}
}

//compare functions (this, other)
/**
 * return 0 if equal
 * 		  1 if this > other
 * 		  -1 if this < other
 */
int compareBool(Object this, Object other){
	return memcmp(this, other, sizeof(bool));
}
int compareUChar(Object this, Object other){
	return memcmp(this, other, sizeof(uchar));
}
int compareChar(Object this, Object other){
	return memcmp(this, other, sizeof(char));
}
int compareUShort(Object this, Object other){
	return memcmp(this, other, sizeof(ushort));
}
int compareShort(Object this, Object other){
	return memcmp(this, other, sizeof(short));
}
int compareInt(Object this, Object other){
	return memcmp(this, other, sizeof(int));
}
int compareUInt(Object this, Object other){
	return memcmp(this, other, sizeof(uint));
}
int compareFloat(Object this, Object other){
	return memcmp(this, other, sizeof(float));
}
int compareULong(Object this, Object other){
	return memcmp(this, other, sizeof(ulong));
}
int compareLong(Object this, Object other){
	return memcmp(this, other, sizeof(long));
}
int compareDouble(Object this, Object other){
	return memcmp(this, other, sizeof(double));
}
int compareString(Object this, Object other){
	return strcmp(this, other);
}

//printer functions
void printBool(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	Bool e = (Bool)p;
	//ternary operator allows for simple true of false write
	fprintf(out, "%s", *e ? "true" : "false");
}
void printUChar(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	UChar e = (UChar)p;
	fprintf(out, "%u", *e);
}
void printChar(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	Char e = (Char)p;
	fprintf(out, "%c", *e);
}
void printUShort(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	UShort e = (UShort)p;
	fprintf(out, "%hu", *e);
}
void printShort(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	Short e = (Short)p;
	fprintf(out, "%h", *e);
}
void printInt(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	Int e = (Int)p;
	fprintf(out, "%d", *e);
}
void printUInt(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	UInt e = (UInt)p;
	fprintf(out, "%u", *e);
}
void printFloat(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	Float e = (Float)p;
	fprintf(out, "%f", *e);
}
void printULong(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	ULong e = (ULong)p;
	fprintf(out, "%lu", *e);
}
void printLong(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	Long e = (Long)p;
	fprintf(out, "%ld", *e);
}
void printDouble(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	Double e = (Double)p;
	fprintf(out, "%f", *e);
}
void printString(FILE* out, Object p){
	if(!out){
		out = stdout;
	}
	if(!p){
		fprintf(out, "NULL");
		return;
	}
	String e = (String)p;
	fprintf(out, "%s", e);
}
void printObject(FILE*out, Object e){
	if(!out){
		out = stdout;
	}
	if(!e){
		fprintf(out, "NULL");
		return;
	}
	fprintf(out, "%p", e);
}
