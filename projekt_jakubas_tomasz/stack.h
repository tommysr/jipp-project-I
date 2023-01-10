#ifndef STACK_H
#define STACK_H

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

typedef void* (*InputData)();
typedef void (*PrintData)(void* data);
typedef void (*FreeData)(void ** data);
typedef DataType(*GetDataType)(void* data);
typedef bool (*IOOperation)(FILE* pf, void** data);
typedef int (*CompareData)(void* curr, void* search);
typedef void (*GetDataMethods)(DataType type, FreeData* free, PrintData* print, IOOperation* save, IOOperation* read, GetDataType* get_type, InputData * input_data);

struct StackElement {
	void * data;
	StackElement * next;

	FreeData free_f;
	IOOperation save_f;
	IOOperation read_f;
	GetDataType get_type_f;
	PrintData print_f;
	InputData input_f;
};

struct Stack {
	size_t length;
	StackElement * last;
};

void freeStack(Stack ** stack);
void printStack(Stack * stack);
void SetMethodsPointers(GetDataMethods methods);
void * searchStack(Stack* stack, StackElement* search, CompareData filter, int first);

Stack * pushToStack(Stack * stack, StackElement * element);
Stack* createStack();

StackElement popFromStack(Stack** stack);
StackElement * lastFromStack(Stack * stack);

bool saveStack(Stack * stack, const char * fileName);
bool readStack(Stack ** stack, const char * fileName);

bool isEmpty(Stack * stack);


#endif

