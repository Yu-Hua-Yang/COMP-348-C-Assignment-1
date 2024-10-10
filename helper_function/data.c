#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "student.h"

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

Student* readFile() {
    const char *filename = "students.txt"; // Hardcoded filename
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Dynamically allocate an initial array size
    size_t capacity = 2; // Start with a small capacity
    Student *students = malloc(capacity * sizeof(Student));
    if (students == NULL) {
        perror("Error allocating memory for students array");
        fclose(file);
        return NULL;
    }

    size_t studentCount = 0; // Counter for the number of students

    // Read the file line by line
    char *line = NULL;
    size_t bufferSize = 0;
    while (getline(&line, &bufferSize, file) != -1) {
        // If we reach capacity, expand the array using realloc
        if (studentCount >= capacity) {
            capacity *= 2; // Double the capacity
            students = realloc(students, capacity * sizeof(Student));
            if (students == NULL) {
                perror("Error reallocating memory for students array");
                free(line);
                fclose(file);
                return NULL;
            }
        }

        // Parse each line and store the data in the array of students
        parseLine(line, &students[studentCount]);
        studentCount++;
    }

    // Clean up
    free(line);  // Free the line buffer
    fclose(file); // Close the file

    // Resize the array to fit the exact number of students read
    students = realloc(students, studentCount * sizeof(Student));
    return students; // Return the array of students
}

void DisplaySpreadSheet(Student *students) {
    // Print the header
    printf("COMP 348 GRADE SHEET\n"
          "\n");
    printf("ID    Last        First       A1   A2   A3   Midterm   Exam   Total   Grade\n");
    printf("----  ----------  ----------  ---- ---- ---- -------- ------- ------  -----\n");

    // Print each student's data until we hit the sentinel (ID == 0)
    for (size_t i = 0; ; i++) {
        if (students[i].id == 0)  // Sentinel value to stop printing
            break;

        // Calculate total and grade for the student
        float total = calculateTotal(students[i]);
        char grade = calculateGrade(total);

        // Print the student data in the required format
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
    // Calculate weighted contributions
    float assignmentTotal = ((student.asst1 + student.asst2 + student.asst3) / 120.0) * 25.0;
    float midtermTotal = (student.midterm / 25.0) * 25.0;
    float finalTotal = (student.final / 40.0) * 50.0;

    // Total grade is the sum of the weighted contributions
    return assignmentTotal + midtermTotal + finalTotal;
}

// Function to assign a grade based on the total percentage
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