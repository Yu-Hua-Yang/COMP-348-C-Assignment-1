#ifndef STUDENT_H  // Include guard to prevent multiple inclusions
#define STUDENT_H

// Structure to hold student data
typedef struct {
    int id;
    char lastName[50];
    char firstName[50];
    int asst1;
    int asst2;
    int asst3;
    int midterm;
    int final;
} Student;

#endif // STUDENT_H