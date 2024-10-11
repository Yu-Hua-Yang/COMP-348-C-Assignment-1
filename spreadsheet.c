#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "spreadsheet.h"
#include "student.h"

extern GradeScheme currentScheme;

/**
 * Initializes the spreadsheet.
 */
void initSpreadsheet(Spreadsheet* sheet) {
    if (sheet == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to initSpreadsheet\n");
        return;
    }
    sheet->studentCount = 0;
    sheet->sortColumn = 0; // Default sort by ID
}

/**
 * Loads students from a file into the spreadsheet.
 */
int loadStudents(Spreadsheet* sheet, const char* filename) {
    if (sheet == NULL || filename == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to loadStudents\n");
        return -1;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename, strerror(errno));
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) && sheet->studentCount < MAX_STUDENTS) {
        Student* s = &sheet->students[sheet->studentCount];
        int result = sscanf(line, "%d|%49[^|]|%49[^|]|%d|%d|%d|%d|%d",
                            &s->id, s->lastName, s->firstName,
                            &s->assignmentGrades[0], &s->assignmentGrades[1], &s->assignmentGrades[2],
                            &s->midtermGrade, &s->examGrade);
        if (result != 8) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }
        calculateTotalGrade(s);
        assignLetterGrade(s);
        sheet->studentCount++;
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file '%s': %s\n", filename, strerror(errno));
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

/**
 * Displays the spreadsheet.
 */
void displaySpreadsheet(const Spreadsheet* sheet) {
    if (sheet == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to displaySpreadsheet\n");
        return;
    }

    printf("%-8s %-15s %-15s %-5s %-5s %-5s %-8s %-6s %-6s %-6s\n", 
           "ID", "Last Name", "First Name", "A1", "A2", "A3", "Midterm", "Exam", "Total", "Grade");
    for (int i = 0; i < sheet->studentCount; i++) {
        const Student* s = &sheet->students[i];
        printf("%-8d %-15s %-15s %-5d %-5d %-5d %-8d %-6d %-6.2f %-6c\n",
               s->id, s->lastName, s->firstName,
               s->assignmentGrades[0], s->assignmentGrades[1], s->assignmentGrades[2],
               s->midtermGrade, s->examGrade, s->totalGrade, s->letterGrade);
    }
}

/**
 * Sorts the spreadsheet based on the specified sort column.
 */
void sortSpreadsheet(Spreadsheet* sheet) {
    if (sheet == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to sortSpreadsheet\n");
        return;
    }
    qsort_r(sheet->students, sheet->studentCount, sizeof(Student), compareStudents, sheet->sortColumn);
}

void displayHistogram(const Spreadsheet* sheet) {
    if (sheet == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to displayHistogram\n");
        return;
    }

    int gradeCounts[5] = {0}; // A, B, C, D, F
    for (int i = 0; i < sheet->studentCount; i++) {
        switch (sheet->students[i].letterGrade) {
            case 'A': gradeCounts[0]++; break;
            case 'B': gradeCounts[1]++; break;
            case 'C': gradeCounts[2]++; break;
            case 'D': gradeCounts[3]++; break;
            case 'F': gradeCounts[4]++; break;
            default: fprintf(stderr, "Error: Invalid letter grade %c\n", sheet->students[i].letterGrade); break;
        }
    }

    printf("Grade Distribution:\n");
    printf("A: %d\n", gradeCounts[0]);
    printf("B: %d\n", gradeCounts[1]);
    printf("C: %d\n", gradeCounts[2]);
    printf("D: %d\n", gradeCounts[3]);
    printf("F: %d\n", gradeCounts[4]);
}

int compareStudents(const void* a, const void* b, int sortColumn) {
    const Student* studentA = (const Student*)a;
    const Student* studentB = (const Student*)b;

    switch (sortColumn) {
        case 0: return studentA->id - studentB->id;
        case 1: return strcmp(studentA->lastName, studentB->lastName);
        case 2: return strcmp(studentA->firstName, studentB->firstName);
        case 3: return studentA->totalGrade - studentB->totalGrade;
        default: fprintf(stderr, "Error: Invalid sort column %d\n", sortColumn); return 0;
    }
}


void updateStudentName(Spreadsheet* sheet, int studentId, const char* newLastName) {
    if (sheet == NULL || newLastName == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to updateStudentName\n");
        return;
    }

    for (int i = 0; i < sheet->studentCount; i++) {
        if (sheet->students[i].id == studentId) {
            strncpy(sheet->students[i].lastName, newLastName, MAX_NAME_LENGTH);
            sheet->students[i].lastName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
            return;
        }
    }
    printf("Student ID not found.\n");
}

void updateStudentGrade(Spreadsheet* sheet, int studentId, int examGrade) {
    if (sheet == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to updateStudentGrade\n");
        return;
    }

    for (int i = 0; i < sheet->studentCount; i++) {
        if (sheet->students[i].id == studentId) {
            if (examGrade >= 0 && examGrade <= 40) {
                sheet->students[i].examGrade = examGrade;
                calculateTotalGrade(&sheet->students[i]);
                assignLetterGrade(&sheet->students[i]);
            } else {
                printf("Invalid exam grade. Must be between 0 and 40.\n");
            }
            return;
        }
    }
    printf("Student ID not found.\n");
}

void changeGradeScheme(Spreadsheet* sheet, GradeScheme newScheme) {
    if (sheet == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to changeGradeScheme\n");
        return;
    }

    currentScheme = newScheme;
    for (int i = 0; i < sheet->studentCount; i++) {
        assignLetterGrade(&sheet->students[i]);
    }
}


void removeStudent(Spreadsheet* sheet, int studentId) {
    if (sheet == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to removeStudent\n");
        return;
    }

    int index = -1;
    for (int i = 0; i < sheet->studentCount; i++) {
        if (sheet->students[i].id == studentId) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Student ID not found.\n");
        return;
    }

    for (int i = index; i < sheet->studentCount - 1; i++) {
        sheet->students[i] = sheet->students[i + 1];
    }
    sheet->studentCount--;
}