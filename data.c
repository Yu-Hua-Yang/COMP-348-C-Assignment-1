#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "student.h"
#include "spreadsheet.h"

#define MAX_STUDENTS 1000

int sortColumn = 1; // Default sort column (1 = Student ID)

// Initial grade mappings
int gradeA = 80;
int gradeB = 70;
int gradeC = 60;
int gradeD = 50;
int gradeF = 0;

/**
 * Compares two students by their student ID.
 *
 * This comparison is used for sorting a list of students.
 *
 * @param a Pointer to the first student.
 * @param b Pointer to the second student.
 *
 * @return A negative value if studentA's ID is less than studentB's ID,
 *         zero if studentA's ID is equal to studentB's ID, and a positive
 *         value if studentA's ID is greater than studentB's ID.
 */
int compareByID(const void *a, const void *b) {
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return (studentA->id - studentB->id);
}

/**
 * Compares two students by their last name.
 *
 * The comparison is case-sensitive and uses the strcmp function.
 *
 * @param a a pointer to the first student
 * @param b a pointer to the second student
 * @return a negative value if studentA's last name is less than studentB's last name,
 *         a positive value if studentA's last name is greater than studentB's last name,
 *         and 0 if the last names are equal.
 */
int compareByLastName(const void *a, const void *b) {
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return strcmp(studentA->lastName, studentB->lastName);
}

/**
 * Compares two students by their final exam grade.
 *
 * The comparison is done in descending order.
 *
 * @param a a pointer to the first student
 * @param b a pointer to the second student
 * @return a negative value if studentA's exam grade is less than studentB's exam grade,
 *         a positive value if studentA's exam grade is greater than studentB's exam grade,
 *         and 0 if the exam grades are equal.
 */
int compareByExamGrade(const void *a, const void *b) {
    int examGradeA = ((Student *)a)->final;
    int examGradeB = ((Student *)b)->final;

    return examGradeB - examGradeA;
}

/**
 * Compares two students by their total course grade.
 *
 * The comparison is done in descending order.
 *
 * @param a a pointer to the first student
 * @param b a pointer to the second student
 * @return a negative value if studentA's total grade is less than studentB's total grade,
 *         a positive value if studentA's total grade is greater than studentB's total grade,
 *         and 0 if the total grades are equal.
 */
int compareByTotal(const void *a, const void *b) {
    float totalA = calculateTotal(*(Student *)a);
    float totalB = calculateTotal(*(Student *)b);
    return (totalB > totalA) - (totalB < totalA);
}


/**
 * Asks the user to choose a column to sort the spreadsheet by.
 *
 * The user is presented with a menu and asked to enter the number of their
 * chosen column. The function loops until the user enters a valid column
 * number (1-4). The chosen column is stored in the global variable sortColumn.
 *
 * @return 1 when the user chose a valid choice if not loops until valid input
 */
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

/**
 * Displays the spreadsheet of students and their grades.
 *
 * This function takes a pointer to the start of an array of Student structs as
 * input and displays the spreadsheet in the console. The spreadsheet is sorted
 * according to the global variable sortColumn.
 *
 * The spreadsheet is displayed in the following format:
 *
 * ID    Last        First       A1   A2   A3   Midterm   Exam   Total   Grade
 * ----  ----------  ----------  ---- ---- ---- -------- ------- ------  -----
 * <id>  <lastName>  <firstName> <A1> <A2> <A3> <midterm> <exam> <total> <grade>
 *
 * The grade is calculated by calling calculateGrade on the total.
 * The total is calculated by calling calculateTotal on the student grades.
 *
 * @param students Pointer to the array of Student structs.
 */
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
            qsort(students, numStudents, sizeof(Student), compareByExamGrade);
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

/**
 * Calculates the total course grade for a student.
 *
 * The total course grade is a weighted sum of the assignment, midterm and final grades.
 * The weights are as follows: assignment 25%, midterm 25%, final 50%.
 *
 * @param student The student for whom to calculate the total course grade.
 * @return The total course grade for the student.
 */
float calculateTotal(Student student) {
    float assignmentTotal = ((student.asst1 + student.asst2 + student.asst3) / 120.0) * 25.0;
    float midtermTotal = (student.midterm / 25.0) * 25.0;
    float finalTotal = (student.final / 40.0) * 50.0;

    return assignmentTotal + midtermTotal + finalTotal;
}

char calculateGrade(float total) {
    if (total >= gradeA) {
        return 'A';
    } else if (total >= gradeB) {
        return 'B';
    } else if (total >= gradeC) {
        return 'C';
    } else if (total >= gradeD) {
        return 'D';
    } else {
        return 'F';
    }
}

/**
 * Reads the student data from a file and returns an array of Student structures.
 *
 * The file is expected to be in the following format:
 *   Student ID|Last name|First name|Assignment 1|Assignment 2|Assignment 3|Midterm|Final
 *   ...
 *
 * The function allocates memory for the students array and populates it with the
 * student data from the file. If the file cannot be opened or if there is an error
 * allocating memory, the function returns NULL.
 *
 * @return An array of Student structures, or NULL if there is an error.
 */
Student* readFile() {
    const char *filename = "students.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    Student *students = malloc(MAX_STUDENTS * sizeof(Student));
    if (students == NULL) {
        perror("Error allocating memory for students array");
        fclose(file);
        return NULL;
    }

    size_t studentCount = 0;
    char *line = NULL;
    size_t bufferSize = 0;

    while (getline(&line, &bufferSize, file) != -1 && studentCount < MAX_STUDENTS) {
        parseLine(line, &students[studentCount]);
        studentCount++;
    }

    free(line);
    fclose(file);

    return students;
}

/**
 * Writes the given array of Student structures to a file.
 *
 * The function opens the file named by the global constant filename in
 * overwrite mode and writes each student's data to the file in the following
 * format:
 *   Student ID|Last name|First name|Assignment 1|Assignment 2|Assignment 3|Midterm|Final
 *   ...
 *
 * If the file cannot be opened, the function prints an error message and
 * returns -1. If the write is successful, the function returns 0.
 *
 * @param students An array of Student structures.
 * @return 0 if the write is successful, -1 if there is an error.
 */
int writeToFile(Student *students) {
    const char *filename = "students.txt";
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return -1;
    }

    size_t i = 0;
    while (students[i].id != 0 && i < MAX_STUDENTS) {
        fprintf(file, "%d|%s|%s|%d|%d|%d|%d|%d\n",
                students[i].id,
                students[i].lastName,
                students[i].firstName,
                students[i].asst1,
                students[i].asst2,
                students[i].asst3,
                students[i].midterm,
                students[i].final);
        i++;
    }

    fclose(file);
    return 0;
}


/**
 * Parses a line of text into a Student structure.
 *
 * The line is expected to be in the following format:
 *   Student ID|Last name|First name|Assignment 1|Assignment 2|Assignment 3|Midterm|Final
 *
 * This function uses sscanf to parse the line into the fields of the Student
 * structure.
 *
 * @param line The line to parse.
 * @param student The Student structure to populate with the parsed data.
 */
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

/**
 * Prints a line of asterisks to the console.
 *
 * @param count The number of asterisks to print.
 */
void printAsterisks(int count) {
    for (int i = 0; i < count; i++) {
        printf("*");
    }
    printf("\n");
}

/**
 * Displays the grade distribution of the students.
 *
 * This function takes a pointer to an array of Student structs as input and
 * displays the grade distribution in the console. The grade distribution is
 * displayed as a bar chart with asterisks, one for each letter grade.
 *
 * @param students Pointer to the array of Student structs.
 */
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

/**
 * Updates a student's last name.
 *
 * This function takes a pointer to an array of Student structs as input and
 * prompts the user to enter a valid student ID. If the student ID is found, the
 * function prompts the user to enter a new last name and updates the student's
 * last name in the array. The function saves the changes to the file and
 * displays a confirmation message.
 *
 * If the student ID is not found, the function prompts the user to enter a
 * valid student ID again.
 *
 * @param students Pointer to the array of Student structs.
 */
void updateLastName(Student *students) {  
    int studentID;
    int found = 0; // Variable to track if the student is found

    // Loop until a valid student ID is entered
    while (!found) {
      // Display the current spreadsheet
      displaySpreadSheet(students);

      printf("Enter Student ID: ");
      scanf("%d", &studentID);
      printf("\n");

      // Search for the student by ID
      size_t i = 0;
      while (students[i].id != 0) {
          if (students[i].id == studentID) {
              // Student found, prompt for new last name
              char newLastName[50]; // Adjust size as needed
              printf("Enter updated last name: ");
              scanf("%s", newLastName);

              // Update last name
              strcpy(students[i].lastName, newLastName);
              printf("Last name updated\n\n");

              // Save the changes to the file
              if (writeToFile(students) != 0) {
                  printf("Error updating the file.\n");
              }

              found = 1; // Set found to true, break out of the loop
              break;
          }
          i++;
      }

      // If student not found, prompt the user again
      if (!found) {
        clearInputBuffer();
        system("clear");
        printf("Student ID %d not found. Please try again.\n\n", studentID);
      }
  }
}

/**
 * Updates a student's exam grade.
 *
 * This function takes a pointer to an array of Student structs as input and
 * prompts the user to enter a valid student ID. If the student ID is found, the
 * function prompts the user to enter a new exam grade and updates the student's
 * exam grade in the array. The function saves the changes to the file and
 * displays a confirmation message.
 *
 * If the student ID is not found, the function prompts the user to enter a
 * valid student ID again.
 *
 * @param students Pointer to the array of Student structs.
 */
void updateExamGrade(Student *students) {  
    int studentID;
    int found = 0; // Variable to track if the student is found

    // Loop until a valid student ID is entered
    while (!found) {
        // Display the current spreadsheet
        displaySpreadSheet(students);

        printf("Enter Student ID: ");
        scanf("%d", &studentID);
        printf("\n");

        // Search for the student by ID
        size_t i = 0;
        while (students[i].id != 0) {
            if (students[i].id == studentID) {
                int isGradeValid = 0;
                int newGrade;

                // Loop until a valid grade is entered
                while (!isGradeValid) {
                    printf("Enter updated exam grade: ");
                    scanf("%d", &newGrade);

                    // Validate grade input
                    if (newGrade >= 0 && newGrade <= 40) {
                        isGradeValid = 1;
                    } else {
                        clearInputBuffer();
                        system("clear");
                        printf("Invalid grade. Please try again.\n\n");
                    }
                }

                // Update the student's final exam grade
                students[i].final = newGrade;
                printf("Exam grade updated\n\n");

                // Save the changes to the file
                if (writeToFile(students) != 0) {
                    printf("Error updating the file.\n");
                }

                found = 1; // Set found to true, break out of the loop
                break;
            }
            i++;
        }

        // If student not found, prompt the user again
        if (!found) {
            clearInputBuffer();
            system("clear");
            printf("Student ID %d not found. Please try again.\n\n", studentID);
        }
    }
}

/**
 * Prompts the user to enter a new grade baseline for a given grade name and
 * validates the input to ensure it is within the range of 0 to 100.
 *
 * The function will loop until a valid input is entered.
 *
 * @param gradeName The name of the grade to be updated (A, B, C, D, F).
 * @return The new grade baseline.
 */
int getValidatedGrade(const char* gradeName) {
    int isValid = 0;
    int newGrade;

    while (!isValid)
    {
      printf("Enter new %s baseline (0-100): ", gradeName);
        scanf("%d", &newGrade);
        if (newGrade >= 0 && newGrade <= 100) {
          isValid = 1;
        } else {
          clearInputBuffer();
          system("clear");
          printf("Invalid input. Please enter a value between 0 and 100.\n");
        }
    }
    
    return newGrade;
}

/**
 * Updates the grade mappings for the assignment.
 *
 * This function prompts the user to enter a new grade mapping for each grade
 * (A, B, C, D, F) and stores the new mappings in the global variables gradeA,
 * gradeB, gradeC, gradeD, and gradeF. The function also displays the current
 * and new mappings.
 */
void updateGradeMappings() {
    // Display current mappings
    printf("Current Mapping:\n");
    printf(" A: >= %d\n", gradeA);
    printf(" B: >= %d\n", gradeB);
    printf(" C: >= %d\n", gradeC);
    printf(" D: >= %d\n", gradeD);
    printf(" F: < %d\n", gradeD);
    printf("\n");

    // Get new grade mappings
    gradeA = getValidatedGrade("A");
    gradeB = getValidatedGrade("B");
    gradeC = getValidatedGrade("C");
    gradeD = getValidatedGrade("D");

    // Display new mappings
    printf("\nNew Mapping:\n");
    printf("A: >= %d\n", gradeA);
    printf("B: >= %d\n", gradeB);
    printf("C: >= %d\n", gradeC);
    printf("D: >= %d\n", gradeD);
    printf("F: < %d\n", gradeD);
    printf("\n");
}

/**
 * Removes a student from the spreadsheet.
 *
 * This function takes a pointer to an array of Student structs as input and
 * prompts the user to enter a valid student ID. If the student ID is found, the
 * function removes the student from the array, shifts all the students after the
 * removed student to the left, and marks the last student as removed by setting
 * its ID to 0. The function saves the updated student list to the file and
 * displays a confirmation message.
 *
 * If the student ID is not found, the function prompts the user to enter a valid
 * student ID again.
 *
 * @param students Pointer to the array of Student structs.
 */
void removeStudent(Student *students) {
    int studentID;
    int found = 0;  // Track if the student is found
    size_t numStudents = 0;

    // Count the number of students
    while (students[numStudents].id != 0 && numStudents < MAX_STUDENTS) {
        numStudents++;
    }

    // Loop until a valid student ID is entered
    while (!found) {
        // Display the current spreadsheet
        displaySpreadSheet(students);

        printf("Enter Student ID to remove: ");
        scanf("%d", &studentID);
        printf("\n");

        // Search for the student by ID
        for (size_t i = 0; i < numStudents; i++) {
            if (students[i].id == studentID) {
                found = 1;  // Student found

                // Shift all the students after the found student to the left
                for (size_t j = i; j < numStudents - 1; j++) {
                    students[j] = students[j + 1];
                }

                // Mark the last student as removed by setting its ID to 0
                students[numStudents - 1].id = 0;

                printf("Student successfully deleted.\n\n");

                // Save the updated student list to the file
                if (writeToFile(students) != 0) {
                    printf("Error updating the file.\n");
                }

                break;
            }
        }

        // If student not found, prompt again
        if (!found) {
            clearInputBuffer();
            system("clear");
            printf("Student ID %d not found. Please try again.\n\n", studentID);
        }
    }
}