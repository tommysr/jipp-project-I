#include "errors.h"

static const char * status_messages[] = {
	"E error: memory allocation error",
	"E error: io read error",
	"E error: cannot pop data",
	"E error: cannot push data",
	"E error: stack creation error",
	"E error: io save error",

	"I info: no search data found in stack",
	"I info: stack is empty",
	"I info: stack is free",
	"I info: unknown type of object",
	"I info: unknown operation",
	"I info: stack was freed",
	"I info: stack was saved to file",
	"I info: stack was read from file",
	"I info: popped data from the stack",
	"I info: pushed data to the stack",
	"I info: printed stack:",
	"I info: last item in stack:"
};

int print(STATUS error) {
	puts(status_messages[error] + 2);

	if (status_messages[error][0] == 'E')
	{
		return 1;
	}

	return 0;
}