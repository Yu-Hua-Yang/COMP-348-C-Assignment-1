#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "student.h"

// Constants
#define MAX_STUDENTS 300
#define MAX_LINE_LENGTH 256
#define NUM_ASSIGNMENTS 3
#define MAX_GRADE 40
#define MAX_MIDTERM_GRADE 25
#define MAX_ASSIGNMENT_GRADE 120
#define MAX_TOTAL_GRADE 100

typedef struct {
    int A, B, C, D;
} GradeScheme;

typedef struct {
    Student students[MAX_STUDENTS];
    int studentCount;
    int sortColumn;
} Spreadsheet;

// Function prototypes
void initSpreadsheet(Spreadsheet* sheet);
int loadStudents(Spreadsheet* sheet, const char* filename);
void displaySpreadsheet(const Spreadsheet* sheet);
void sortSpreadsheet(Spreadsheet* sheet);
void displayHistogram(const Spreadsheet* sheet);
void updateStudentName(Spreadsheet* sheet, int studentId, const char* newLastName);
void updateStudentGrade(Spreadsheet* sheet, int studentId, int examGrade);
void changeGradeScheme(Spreadsheet* sheet, GradeScheme newScheme);
void removeStudent(Spreadsheet* sheet, int studentId);
int compareStudents(const void* a, const void* b, int sortColumn);

#endif