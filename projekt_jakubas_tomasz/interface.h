#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#include "data.h"
#include "stack.h"
#include "errors.h"

enum INTERFACE {
	INTERF_PUSH,
	INTERF_POP,
	INTERF_LAST,
	INTERF_SAVE,
	INTERF_READ,
	INTERF_FREE,
	INTERF_SEARCH,
	INTERF_PRINT,
	INTERF_STOP,
	INTERF_COUNT
};


void menu();
void showTypes();
void deallock(void * data);
void last(Stack * stack);
void print(Stack * stack);
void close(Stack * stack);
void search(Stack * stack);
void save(Stack * stack, const char* fileName);
void GetFunctionPointers(DataType type, FreeData * free, PrintData * print, IOOperation * save, IOOperation * read, GetDataType * get_type, InputData * input_data);


Stack * getStack();
Stack * pop(Stack* stack);
Stack * add(Stack* stack);
Stack * free(Stack* stack);
Stack * read(Stack* stack, const char* fileName);

#endif