#ifndef DATASTORAGE_H
#define DATASTORAGE_H
#include "student.h"

Student* readFile();
void DisplaySpreadSheet(Student* students); // Function prototype
float calculateTotal(Student student);
char calculateGrade(float total);
#endif // DATASTORAGE_H
