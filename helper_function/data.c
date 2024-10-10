#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "student.h"
#include "spreadsheet.h"

int sortColumn = 1; // Default sort column (1 = Student ID)

// Comparison functions
int compareByID(const void *a, const void *b) {
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return (studentA->id - studentB->id);
}

int compareByLastName(const void *a, const void *b) {
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return strcmp(studentA->lastName, studentB->lastName);
}

// Comparison function for sorting by letter grade
int compareByGrade(const void *a, const void *b) {
    float totalA = calculateTotal(*(Student *)a);
    float totalB = calculateTotal(*(Student *)b);
    char gradeA = calculateGrade(totalA);
    char gradeB = calculateGrade(totalB);

    // Sort by letter grade
    return gradeA - gradeB; // Ascending order by letter grade
}

int compareByTotal(const void *a, const void *b) {
    float totalA = calculateTotal(*(Student *)a);
    float totalB = calculateTotal(*(Student *)b);
    return (totalB > totalA) - (totalB < totalA); // Descending order
}

// Function to select the sort column
int selectSortColumn() {
    int sortChosen = 0;
    int choice;
    while (!sortChosen) {
        printf("Column Options\n");
        printf("--------------\n");
        printf("1. Student ID\n");
        printf("2. Last name\n");
        printf("3. Exam\n");
        printf("4. Total\n\n");
        printf("Sort Column: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= 4) {
            sortColumn = choice;
            printf("Sort column updated\n\n");
            sortChosen = 1;  // Exit loop when valid input is given
        } else {
            clearInputBuffer();
            system("clear");
            printf("Invalid choice. Please try again.\n\n");
        }
    }
    return sortChosen;
}

// Modified DisplaySpreadSheet function
void displaySpreadSheet(Student *students) {
    size_t numStudents = 0;
    while (students[numStudents].id != 0) {
        numStudents++;
    }

    // Sort based on the chosen sort column
    switch (sortColumn) {
        case 1:
            qsort(students, numStudents, sizeof(Student), compareByID);
            break;
        case 2:
            qsort(students, numStudents, sizeof(Student), compareByLastName);
            break;
        case 3:
            qsort(students, numStudents, sizeof(Student), compareByGrade);
            break;
        case 4:
            qsort(students, numStudents, sizeof(Student), compareByTotal);
            break;
    }

    // Display the sorted spreadsheet
    printf("COMP 348 GRADE SHEET\n\n");
    printf("ID    Last        First       A1   A2   A3   Midterm   Exam   Total   Grade\n");
    printf("----  ----------  ----------  ---- ---- ---- -------- ------- ------  -----\n");

    for (size_t i = 0; i < numStudents; i++) {
        float total = calculateTotal(students[i]);
        char grade = calculateGrade(total);

        printf("%-4d  %-10s  %-10s  %-4d %-4d %-4d %-8d %-7d %-7.2f  %c\n",
               students[i].id,
               students[i].lastName,
               students[i].firstName,
               students[i].asst1,
               students[i].asst2,
               students[i].asst3,
               students[i].midterm,
               students[i].final,
               total,
               grade);
    }
    printf("\n");
}

float calculateTotal(Student student) {
    float assignmentTotal = ((student.asst1 + student.asst2 + student.asst3) / 120.0) * 25.0;
    float midtermTotal = (student.midterm / 25.0) * 25.0;
    float finalTotal = (student.final / 40.0) * 50.0;

    return assignmentTotal + midtermTotal + finalTotal;
}

char calculateGrade(float total) {
    if (total >= 80) {
        return 'A';
    } else if (total >= 70) {
        return 'B';
    } else if (total >= 60) {
        return 'C';
    } else if (total >= 50) {
        return 'D';
    } else {
        return 'F';
    }
}

Student* readFile() {
    const char *filename = "students.txt"; // Hardcoded filename
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    size_t capacity = 2;
    Student *students = malloc(capacity * sizeof(Student));
    if (students == NULL) {
        perror("Error allocating memory for students array");
        fclose(file);
        return NULL;
    }

    size_t studentCount = 0;

    char *line = NULL;
    size_t bufferSize = 0;
    while (getline(&line, &bufferSize, file) != -1) {
        if (studentCount >= capacity) {
            capacity *= 2;
            students = realloc(students, capacity * sizeof(Student));
            if (students == NULL) {
                perror("Error reallocating memory for students array");
                free(line);
                fclose(file);
                return NULL;
            }
        }

        parseLine(line, &students[studentCount]);
        studentCount++;
    }

    free(line);
    fclose(file);

    students = realloc(students, studentCount * sizeof(Student));
    return students;
}

void parseLine(char *line, Student *student) {
    sscanf(line, "%d|%[^|]|%[^|]|%d|%d|%d|%d|%d",
           &student->id,
           student->lastName,
           student->firstName,
           &student->asst1,
           &student->asst2,
           &student->asst3,
           &student->midterm,
           &student->final);
}

// Function to display the grade distribution (Option 2)
void printAsterisks(int count) {
    for (int i = 0; i < count; i++) {
        printf("*");
    }
    printf("\n");
}

void displayGradeDistribution(Student *students) {
    int countA = 0, countB = 0, countC = 0, countD = 0, countF = 0;

    size_t i = 0;
    while (students[i].id != 0) {
        float total = calculateTotal(students[i]);
        char grade = calculateGrade(total);

        switch (grade) {
            case 'A':
                countA++;
                break;
            case 'B':
                countB++;
                break;
            case 'C':
                countC++;
                break;
            case 'D':
                countD++;
                break;
            case 'F':
                countF++;
                break;
        }
        i++;
    }

    // Display the grade distribution
    printf("COMP 348 Grade Distribution\n\n");

    printf("A: ");
    printAsterisks(countA);

    printf("B: ");
    printAsterisks(countB);

    printf("C: ");
    printAsterisks(countC);

    printf("D: ");
    printAsterisks(countD);

    printf("F: ");
    printAsterisks(countF);

    printf("\n");
}