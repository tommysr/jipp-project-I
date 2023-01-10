#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

enum STATUS {
	//Errors
	ALOC_ERROR,
	IO_READ_ERROR,
	IO_SAVE_ERROR,
	POP_DATA_ERROR,
	PUSH_DATA_ERROR,
	STACK_CREATION_ERROR,

	//Infos
	INFO_NO_DATA_FOUND,
	INFO_EMPTY_STACK,
	INFO_STACK_FREE,
	INFO_UNKNOWN_TYPE_OF_OBJECT,
	INFO_UNKNOWN_OPERATION,
	INFO_STACK_FREED,
	INFO_STACK_SAVED,
	INFO_STACK_READ,
	INFO_POPPED_DATA,
	INFO_PUSHED_DATA,
	INFO_PRINTED_DATA,
	INFO_GET_LAST,

	//Count
	STATUSES_COUNT
};

int print(STATUS error);

#endif