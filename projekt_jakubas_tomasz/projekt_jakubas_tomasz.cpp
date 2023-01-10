#include <stdlib.h>
#include <stdio.h>

#include "interface.h"

#pragma warning (disable : 4996)

int main(int argc, char* argv[])
{
	int operation = -1, ret;
	SetMethodsPointers(GetFunctionPointers);
	Stack * new_stack = getStack();
	const char fileName[] = "my_stack.bin";

	while (true)
	{
		menu();
		printf("Choose operation: ");
		
		ret = scanf_s("%d", &operation);
		printf("\n");

		if (operation < INTERF_PUSH || operation > INTERF_STOP)
		{
			flush_in();
			continue;
		}
			
		switch (operation)
		{
		case INTERF_PUSH: 
			new_stack = add(new_stack);
			break;
		case INTERF_POP: 
			new_stack = pop(new_stack);
			break;
		case INTERF_LAST:
			last(new_stack);
			break;
		case INTERF_SAVE:
			save(new_stack, fileName);
			break;
		case INTERF_READ:
			new_stack = read(new_stack, fileName);
			break;
		case INTERF_FREE:
			new_stack = free(new_stack);
			break;
		case INTERF_SEARCH:
			search(new_stack);
			break;
		case INTERF_PRINT:
			print(new_stack);
			break;
		case INTERF_STOP:
			close(new_stack);
			return 0;
		default:
			print(INFO_UNKNOWN_OPERATION);
			close(new_stack);
		};

		operation = -1;
	}

	new_stack = free(new_stack);
	return 0;
}

