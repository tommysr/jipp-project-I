#include "data.h"

#pragma warning (disable : 4996)

static const char* field_of_study[] =
{
	"Computer science",			 //COMPUTER_SCIENCE
	"Biotechnology",			 //BIOTECHNOLOGY
	"Telecomunication",			 //TELECOMUNICATION
	"Physics"					 //PHYSICS
};

const char* getFieldOfStudyName(FieldOfStudy field) {
	return field_of_study[(size_t)field];
}

void showFieldsOfStudies() {
	size_t it;
	printf("FIELDS OF STUDIES:\n");
	for (it = 0; it < FIELD_OF_STUDY_COUNT; ++it)
	{
		printf("%zd %s\n", it, field_of_study[it]);
	}
}

void* createStudent(char* lastname, int birth_year, FieldOfStudy field_of_study)
{
	Student * student = (Student *)malloc(sizeof(Student));

	if (!student) {
		return NULL;
	}

	size_t len = strlen(lastname) + 1;
	char * temp = (char*)malloc(len * sizeof(char));
	
	if (!temp) {
		free(student);
		return NULL;
	}
	
	temp[len - 1] = '\0';
	strcpy_s(temp, len * sizeof(char),  lastname);

	student->lastname = temp;
	student->birth_year = birth_year;
	student->len = (unsigned int)(len - 1);
	student->field_of_study = field_of_study;
	
	return (void*)(student);
}

void flush_in() {
	while (getchar() != '\n');
}

void * inputStudent()
{
	char lastname[128];
	int year;
	int field = -1;

	flush_in();
	printf("Enter lastname of the student: ");
	int ret = scanf_s("%s", lastname, (unsigned int)sizeof(lastname));

	printf("Enter birth year: ");
	ret = scanf_s("%d", &year);

	while (true) {
		flush_in();
		showFieldsOfStudies();
		printf("Choose field of studies: ");

		ret = scanf_s("%d", &field);
		printf("\n");

		if (field < COMPUTER_SCIENCE || field > PHYSICS)
			field = -1;
		else
			break;
	}
	
	flush_in();
	return createStudent(lastname, year, (FieldOfStudy)field);
}


void freeStudent(void** ptr)
{
	if (ptr)
	{
		if (*ptr)
		{
			Student * student = (Student *)(* ptr);

			if (student->lastname)
			{
				free(student->lastname);
				student->lastname = NULL;
			}

			free(student);
			student = NULL;

			*ptr = student;
		}
	}
}

void * pushStudent(char* lastname, int birth_year, FieldOfStudy field_of_study)
{
	return createStudent(lastname, birth_year, field_of_study);
}

void printStudent(void* ptr)
{
	Student * student = (Student*)(ptr);

	if (student)
	{
		printf("\nLastname: %s\n", student->lastname);
		printf("Birth year: %d\n", student->birth_year);
		printf("Field of study: %s\n", getFieldOfStudyName(student->field_of_study));
	}
}

int findStudentByLastName(void* data, void* searchData)
{
	Student * curr = (Student*)(data);
	Student * search = (Student*)(searchData);

	if (strcmp(curr->lastname, search->lastname) == 0)
		return 1;

	return 0;
}

bool saveStudent(FILE* pf, void ** pDat)
{
	if (pf && *pDat){
		Student * student = (Student*)(*pDat);

		if (fwrite((const void*)(student), sizeof(Student), 1, pf) != 1)
			return false;

		if (fwrite((const void*)(student->lastname), sizeof(char), (size_t)(student->len), pf) != (size_t)(student->len))
			return false;

		return true;
	} else
		return false;
}


bool readStudent(FILE* pf, void ** data)
{
	if (pf) {
		Student * student = (Student *)(*data);
	
		if (!student) {
			if ((student = (Student *)malloc(sizeof(Student))) == NULL)
				return false;
		}
		
		*data = (void*)(student);
		
		if (fread((void*)(student), sizeof(Student), 1, pf) != 1)
		{
			free(student);
			return false;
		}

		student->lastname = (char*)malloc((size_t)(student->len + 1) * sizeof(char));

		if (!student->lastname) {
			free(student);
			return false;
		}

		memset(student->lastname, 0, (size_t)(student->len + 1) * sizeof(char));

		if (fread((void*)(student->lastname), sizeof(char), student->len, pf) != student->len)
		{
			freeStudent((void **)&student);
			return false;
		}
	
		student->lastname[student->len] = '\0';

		return true;
	} else 
		return false;
}

DataType getTypeStudent(void* pdat)
{
	return DataType::STUDENT;
}
