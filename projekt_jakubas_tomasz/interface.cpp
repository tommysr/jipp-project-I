#include "interface.h"

#pragma warning (disable : 4996)

static const char * type_names[] = {
	"student"			 //STUDENT
};

static const char * menu_options[] = {
	"0 - push",			 //INTERF_PUSH
	"1 - pop",			 //INTERF_POP
	"2 - last",		     //INTERF_FIRST
	"3 - save",			 //INTERF_SAVE
	"4 - read",			 //INTERF_READ
	"5 - free",			 //INTERF_FREE
	"6 - search",		 //INTERF_SEARCH
	"7 - print",		 //INTERF_PRINT
	"8 - finish"         //INTERF_STOP
};

void showTypes() {
	size_t it;

	for (it = 0; it < TYPES_COUNT; ++it) 
	{
		printf("%s\n", type_names[it]);
	}
}

void menu()
{
	size_t it;

	printf("\nMenu:\n");
	for (it = 0; it < INTERF_COUNT; ++it)
	{
		printf("%s\n", menu_options[it]);
	}
}

Stack * getStack() {
	Stack * new_stack = createStack();

	if (!new_stack)
	{
		if (print(STACK_CREATION_ERROR))
			close(new_stack);
	} 

	return new_stack;
}

void search(Stack * stack) {
	if (isEmpty(stack))
	{
		print(INFO_EMPTY_STACK);
		return;
	}

	char lastname[128];
	StackElement search;
	void * found = NULL;
	memset(&search, 0, sizeof(Student));

	printf("\nEnter lastname to search: \n");
	scanf_s("%s", lastname, (unsigned int)sizeof(lastname));

	search.data = createStudent(lastname, NULL, (FieldOfStudy)0);
	search.get_type_f = getTypeStudent;

	found = searchStack(stack, &search, findStudentByLastName, 1);

	if (found){
		printf("Found: \n");
		printStudent(found);
	} else {
		print(INFO_NO_DATA_FOUND);
	}

	while (found)
	{
		found = searchStack(stack, &search, findStudentByLastName, 0);
		if (found)
		{
			printf("Found: ");
			printStudent(found);
		}
	}

	freeStudent(&search.data);
}

Stack * pop(Stack* stack)
{
	if (isEmpty(stack))
		print(INFO_EMPTY_STACK);
	else {
		StackElement popped = popFromStack(&stack);

		if (!popped.data)
		{
			if (print(POP_DATA_ERROR))
				close(stack);
		}
		else {
			print(INFO_POPPED_DATA);
			popped.print_f(popped.data);
			popped.free_f(&popped.data);
		}
	}

	return stack;
}


void last(Stack * stack) {
	if (isEmpty(stack)) 
		print(INFO_EMPTY_STACK); 
	else {
		StackElement * last = lastFromStack(stack);

		print(INFO_GET_LAST);
		(*(stack->last->print_f))(last->data);
	}
}

void save(Stack* stack, const char* fileName)
{
	if (isEmpty(stack)){
		print(INFO_EMPTY_STACK);
		return;
	} 
	
	if (!saveStack(stack, fileName)){
		if (print(IO_SAVE_ERROR))
			close(stack);
	}
	else {
		print(INFO_STACK_SAVED);
	}
}

Stack * read(Stack* stack, const char* fileName)
{
	if (!readStack(&stack, fileName)){
		if (print(IO_READ_ERROR))
			close(stack);
	} else {
		print(INFO_STACK_READ);
	}

	return stack;
}

Stack * add(Stack * stack)
{
	int type = -1;
	DataType data_type = (DataType)0;
	StackElement tmp;
	Student * new_student = NULL;
	void* new_element = NULL;

	memset(&tmp, 0, sizeof(StackElement));

	if (!stack) {
		stack = createStack();

		if (!stack) {
			print(ALOC_ERROR);
			close(stack);
		}
	}

	flush_in();
	printf("Enter type: \n 0 - student: ");
	scanf_s("%d", &type);

	if (type < STUDENT || type >= TYPES_COUNT)
	{
		type = -1;
		flush_in();
	}

	switch (type) {
		case STUDENT:
			new_element = inputStudent();
			new_student = (Student *)new_element;

			if (!new_element) {
				if (print(ALOC_ERROR))
				{
					close(stack);
				}
			}

			tmp.data = new_element;
			tmp.next = NULL;
			tmp.free_f = freeStudent;
			tmp.get_type_f = getTypeStudent;
			tmp.input_f = inputStudent;
			tmp.print_f = printStudent;
			tmp.read_f = readStudent;
			tmp.save_f = saveStudent;


			if ((stack = pushToStack(stack, &tmp)) == NULL)
			{
				if (print(PUSH_DATA_ERROR))
				{
					freeStudent(&new_element);
					close(stack);
				}
			}

			break;
		default:
			print(INFO_UNKNOWN_TYPE_OF_OBJECT);
			return stack;
			break;
	}


	print(INFO_PUSHED_DATA);
	last(stack);
	return stack;
}

Stack * free(Stack* stack)
{
	if (!stack)
		print(INFO_STACK_FREE);
	else {
		freeStack(&stack);
		print(INFO_STACK_FREED);
	}
		
	return stack;
}

void print(Stack * stack)
{
	if (isEmpty(stack))
		print(INFO_EMPTY_STACK);
	else {
		print(INFO_PRINTED_DATA);
		printStack(stack);
	}
}

void close(Stack* stack)
{
	printf("Closing\n");
	freeStack(&stack);
	system("pause");
	exit(1);
}

void deallock(void* object)
{
	if (object)
		free(object);
}

void GetFunctionPointers(DataType type, FreeData* free, PrintData * print,  IOOperation* save, IOOperation* read, GetDataType* get_type, InputData * input_data)
{
	*free = NULL;
	*get_type = NULL;
	*print = NULL;
	*save = NULL;
	*read = NULL;
	*input_data = NULL;

	switch (type) {
	case STUDENT:
		*free = freeStudent;
		*get_type = getTypeStudent;
		*print = printStudent;
		*save = saveStudent;
		*read = readStudent;
		*input_data = inputStudent;
		break;
	default:
		break;
	};
}

