#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "student.h"
#include "spreadsheet.h"

int sortColumn = 1; // Default sort column (1 = Student ID)

// Initial grade mappings
int gradeA = 80;
int gradeB = 70;
int gradeC = 60;
int gradeD = 50;
int gradeF = 0;

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

int compareByExamGrade(const void *a, const void *b) {
    int examGradeA = ((Student *)a)->final;
    int examGradeB = ((Student *)b)->final;

    // Sort by exam grade in descending order
    return examGradeB - examGradeA;
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

int writeToFile(Student *students) {
    const char *filename = "students.txt"; // Hardcoded filename
    FILE *file = fopen(filename, "w"); // Open for writing (overwrite mode)
    if (file == NULL) {
        perror("Error opening file for writing");
        return -1; // Indicate failure
    }

    size_t i = 0;
    while (students[i].id != 0) {
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
    return 0; // Indicate success
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
        printf("Student ID %d not found. Please try again.\n\n", studentID);  // Fix: Pass studentID here
      }
  }
}

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

void removeStudent(Student *students) {
    int studentID;
    int found = 0;  // Track if the student is found
    size_t numStudents = 0;

    // Count the number of students dynamically (assuming student ID of 0 means an empty entry)
    while ((students)[numStudents].id != 0) {
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
            if ((students)[i].id == studentID) {
                found = 1;  // Student found

                // Shift all the students after the found student to the left
                for (size_t j = i; j < numStudents - 1; j++) {
                    (students)[j] = (students)[j + 1];
                }

                // Mark the last student as removed by setting its ID to 0
                (students)[numStudents - 1].id = 0;

                // Resize the array (shrink it by one student)
                numStudents -= 1;
                students = realloc(students, (numStudents + 1) * sizeof(Student)); // +1 for the last empty entry
                if (students == NULL && numStudents > 0) {
                    perror("Error reallocating memory for students array");
                    exit(1);  // Exit on memory reallocation failure
                }

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
