#ifndef STUDENT_H
#define STUDENT_H

#define MAX_NAME_LENGTH 50

typedef struct {
    int id;
    char lastName[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    int assignmentGrades[3];
    int midtermGrade;
    int examGrade;
    float totalGrade;
    char letterGrade;
} Student;

// Function prototypes
void calculateTotalGrade(Student* student);
void assignLetterGrade(Student* student);

#endif