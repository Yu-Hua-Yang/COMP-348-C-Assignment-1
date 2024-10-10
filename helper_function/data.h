#ifndef DATASTORAGE_H
#define DATASTORAGE_H
#include "student.h"

Student* readFile();
void parseLine(char *line, Student *student);
void displaySpreadSheet(Student* students); // Function prototype
void displayGradeDistribution(Student *students);
float calculateTotal(Student student);
char calculateGrade(float total);
int selectSortColumn();
int compareByID(const void *a, const void *b);
int compareByTotal(const void *a, const void *b);
int compareByGrade(const void *a, const void *b);
int compareByLastName(const void *a, const void *b);
#endif // DATASTORAGE_H
