#ifndef DATA_H
#define DATA_H

#include <stdlib.h>
#include <stdio.h>
#include "stdafx.h"
#include "string.h"

enum FieldOfStudy {
	COMPUTER_SCIENCE,
	BIOTECHNOLOGY,
	TELECOMUNICATION,
	PHYSICS,
	FIELD_OF_STUDY_COUNT
};

struct Student {
	char* lastname;
	int birth_year;
	FieldOfStudy field_of_study;
	unsigned int len;
};

void freeStudent(void ** ptr);
void printStudent(void * ptr);
void flush_in();

void* inputStudent();
void* pushStudent(char* lastname, int birth_year, FieldOfStudy field_of_study);
void* createStudent(char* lastname, int birth_year, FieldOfStudy field_of_study);

int findStudentByLastName(void* data, void* searchData);

bool saveStudent(FILE* pf, void **pdat);
bool readStudent(FILE* pf, void **pdat);

DataType getTypeStudent(void* pdat);

#endif