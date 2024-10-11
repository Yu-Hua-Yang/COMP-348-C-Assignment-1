#ifndef DATASTORAGE_H
#define DATASTORAGE_H
#include "student.h"

Student* readFile();
void parseLine(char *line, Student *student);
void displaySpreadSheet(Student* students); // Function prototype
void displayGradeDistribution(Student *students);
void updateGradeMappings();
void updateLastName(Student *students);
void updateExamGrade(Student *students);
void removeStudent(Student **studentsPtr);
float calculateTotal(Student student);
char calculateGrade(float total);
int selectSortColumn();
int getValidatedGrade(const char* gradeName);
int compareByID(const void *a, const void *b);
int compareByTotal(const void *a, const void *b);
int compareByExamGrade(const void *a, const void *b);
int compareByLastName(const void *a, const void *b);
#endif // DATASTORAGE_H
