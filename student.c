#include "student.h"
#include "spreadsheet.h"

GradeScheme currentScheme = {80, 70, 60, 50};

/**
 * Calculates the total grade for a student.
 */
void calculateTotalGrade(Student* student) {
    float assignmentsTotal = 0;
    for (int i = 0; i < NUM_ASSIGNMENTS; i++) {
        assignmentsTotal += student->assignmentGrades[i];
    }
    student->totalGrade = (assignmentsTotal / MAX_ASSIGNMENT_GRADE) * 25 + 
                          (student->midtermGrade / (float)MAX_MIDTERM_GRADE) * 25 + 
                          (student->examGrade / (float)MAX_GRADE) * 50;
}

/**
 * Assigns a letter grade to a student based on the total grade.
 */
void assignLetterGrade(Student* student) {
    if (student->totalGrade >= currentScheme.A) student->letterGrade = 'A';
    else if (student->totalGrade >= currentScheme.B) student->letterGrade = 'B';
    else if (student->totalGrade >= currentScheme.C) student->letterGrade = 'C';
    else if (student->totalGrade >= currentScheme.D) student->letterGrade = 'D';
    else student->letterGrade = 'F';
}