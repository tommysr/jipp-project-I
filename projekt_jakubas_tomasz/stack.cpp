#include "stack.h"

#pragma warning (disable : 4996)

static GetDataMethods get_methods;

void SetMethodsPointers(GetDataMethods methods)
{
	get_methods = methods;
}

bool isEmpty(Stack* stack) {
	if (!stack || !stack->last)
		return true;
	return false;
}

Stack * createStack() {
	Stack* stack = (Stack *)malloc(sizeof(Stack));

	if (!stack)
		return NULL;

	stack->last = NULL; 
	stack->length = 0; 

	return stack;
}

Stack* pushToStack(Stack* stack, StackElement* data) {
	StackElement* new_top = (StackElement*)malloc(sizeof(StackElement));

	if (!new_top)
		return NULL;

	new_top->free_f = data->free_f;
	new_top->input_f = data->input_f;
	new_top->print_f = data->print_f;
	new_top->read_f = data->read_f;
	new_top->save_f = data->save_f;
	new_top->get_type_f = data->get_type_f;
	new_top->next = stack->last;
	new_top->data = data->data;

	stack->length = stack->length + 1;
	stack->last = new_top;

	return stack;
}



void printStack(Stack* stack) {
	if (stack && stack->last)
	{
		StackElement* current = stack->last;

		while (current) {
			(*current->print_f)(current->data);
			current = current->next;
		}
	}
}

StackElement popFromStack(Stack** stack) {
	StackElement popped_element;
	StackElement* new_last = NULL;
	memset(&popped_element, 0, sizeof(StackElement));

	if ((*stack)->last)
	{
		if ((*stack)->last->next) {
			new_last = (*stack)->last->next;
		}

		popped_element.data = (*stack)->last->data;
		popped_element.free_f = (*stack)->last->free_f;
		popped_element.print_f = (*stack)->last->print_f;

		free((*stack)->last);
		(*stack)->last = new_last;
		(*stack)->length = (*stack)->length - 1;
	}

	return popped_element;
}

void freeStack(Stack** stack) {
	if (stack)
	{
		if (*stack)
		{
			StackElement * current = (*stack)->last, * tmp = NULL;

			while (current) {
				(*current->free_f)(&current->data);

				tmp = current;
				current = current->next;

				free(tmp);
			}

			free(*stack);
			*stack = NULL;
		}
	}
}



StackElement * lastFromStack(Stack* stack) {
	return stack->last;
}

void * searchStack(Stack * stack, StackElement * search, CompareData compare_function, int first) {
	static StackElement * current;
	StackElement* tmp = NULL;

	if (!(stack->last))
		return NULL;

	if (first)
		current = stack->last;

	while (current) {
		if ((*current->get_type_f)(&current->data) == (*search->get_type_f)(&search->data))
		{
			if (!(*compare_function)(current->data, search->data))
				current = current->next;
			else
			{
				tmp = current;
				current = current->next;
				return tmp->data;
			}
		}
	}

	return NULL;
}

void stackExit(FILE* pft, Stack * stack, __int64* fdesc)
{
	if (pft)
		fclose(pft);

	freeStack(&stack);

	if (fdesc)
		free(fdesc);

	printf("fatal error\n");
	system("pause");
	exit(1);
}

bool saveStack(Stack* stack, const char * fileName)
{
	if (!stack->last || !fileName)
		return false;

	size_t iterator, noItems = stack->length;
	unsigned int noIt = (unsigned int)noItems;
	__int64 filePos = 0;

	// + 2 for counter and data type
	__int64* fileDescriptor = (__int64*)malloc((noItems + 1) * sizeof(__int64));

	if (!fileDescriptor)
		stackExit(NULL, stack, fileDescriptor);

	FILE* pFile = fopen(fileName, "wb");

	if (!pFile)
		stackExit(pFile, stack, fileDescriptor);

	//store number of items to read
	if (fwrite((const void*)&noIt, sizeof(unsigned int), 1, pFile) != 1)
		stackExit(pFile, stack, fileDescriptor);

	//move pointer after the fileDescriptor location
	_fseeki64(pFile, (noItems + 1) * sizeof(__int64), SEEK_CUR);

	StackElement * current = stack->last;

	for (iterator = 0; iterator < noItems; ++iterator)
	{
		fileDescriptor[iterator] = ftell(pFile);

		DataType type = (*current->get_type_f)(current->data);
		//store data type of stack elements
		if (fwrite((const void*)&type, sizeof(unsigned int), 1, pFile) != 1)
			stackExit(pFile, stack, fileDescriptor);

		//call function to save object;
		if (!(*current->save_f)(pFile, &current->data))
			stackExit(pFile, stack, fileDescriptor);

		current = current->next;
	}

	fileDescriptor[iterator] = ftell(pFile);  
	_fseeki64(pFile, sizeof(unsigned int), SEEK_SET);

	if (fwrite((const void *)fileDescriptor, sizeof(__int64), noItems + 1, pFile) != noItems + 1)
		stackExit(pFile, stack, fileDescriptor);

	if (pFile)
		fclose(pFile);
	pFile = NULL;

	if (fileDescriptor)
		free(fileDescriptor);

	fileDescriptor = NULL;

	return true;
}

bool readStack(Stack** stack, const char* fileName)
{
	if (*stack)
	{
		 freeStack(stack);
	}
	
	unsigned int noItems = 0, iterator, rec;
	__int64* fileDescriptor = NULL;
	StackElement tmp;
	memset((void*)&tmp, 0, sizeof(StackElement));
	*stack = createStack();

	FILE* pFile = fopen(fileName, "rb");
	if (!pFile)
		stackExit(NULL, *stack, fileDescriptor);

	if (fread((void *)&noItems, sizeof(unsigned int), 1, pFile) != 1)
		stackExit(pFile, *stack, fileDescriptor);

	fileDescriptor = (__int64*)malloc((size_t)(noItems + 1) * sizeof(__int64));
	if (!fileDescriptor || !*stack)
		stackExit(pFile, *stack, fileDescriptor);

	if (fread((void *)fileDescriptor, sizeof(fileDescriptor[0]), (size_t)noItems + 1, pFile) != (size_t)noItems + 1)
		stackExit(pFile, *stack, fileDescriptor);

	for (iterator = 0; iterator < noItems; ++iterator)
	{
		rec = noItems - iterator - 1;
	
		_fseeki64(pFile, fileDescriptor[rec], SEEK_SET);

		DataType type;

		if (fread((void*)&type, sizeof(unsigned int), 1, pFile) != 1)
			stackExit(pFile, *stack, fileDescriptor);

		(*get_methods)(type,  &tmp.free_f, &tmp.print_f, &tmp.save_f, &tmp.read_f, &tmp.get_type_f, &tmp.input_f);

		if (!(*tmp.read_f)(pFile, &tmp.data))
			stackExit(pFile, *stack, fileDescriptor);
	
		*stack = pushToStack(*stack, &tmp);

		tmp.data = NULL;
	}
	
	
	if (fileDescriptor)
		free(fileDescriptor);
	fileDescriptor = NULL;
	
	if(pFile)
		fclose(pFile);

	pFile = NULL;	
	return true;
}


