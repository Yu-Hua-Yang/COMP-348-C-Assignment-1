#include <stdio.h>
#include <stdlib.h>
#include "spreadsheet.h"
#include "data.h"
#include "student.h"
#include "print.h"

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Function to handle continuation prompt
int waitForContinue() {
    int isDone = 0;
    char input;

    while (!isDone) {
        printf("Press 'c' or 'C' to continue: ");
        scanf(" %c", &input);  // Get user input
        if (input != 'c' && input != 'C') {
            clearInputBuffer();  // Clear buffer if invalid input
            system("clear");
            printf("Invalid Character. Please try again.\n\n");
        } else {
            isDone = 1;  // Exit loop if valid input
        }
    }
    system("clear");
    return 0;  // Return once the valid input is received
}

int validateSelection() {
    int selectionNumber;
    printf("Selection: ");
    scanf("%d", &selectionNumber);
    system("clear");
    Student* students = readFirle();

    switch (selectionNumber) {
        case 1:
            displaySpreadSheet(students);  // Display the spreadsheet
            waitForContinue();
            return 0;

        case 2:
            displayGradeDistribution(students);  // Display the spreadsheet
            waitForContinue();
            return 0;

        case 3:{
            int isSelected = 0;
            while (!isSelected) {
               isSelected = selectSortColumn();
            }
            waitForContinue();
            return 0;
        }
        case 4:
            updateLastName(students);
            waitForContinue();
            return 0;
        case 5:
            updateExamGrade(students);
            waitForContinue();
            return 0;
        case 6:
            updateGradeMappings();
            waitForContinue();
            return 0;
        case 7:
            removeStudent(&students);
            waitForContinue();
            return 0;
        case 8:
            printf("Goodbye and thanks for using our spreadsheet app");
            return 1;

        default:
            // Handle invalid selection numbers
            clearInputBuffer();
            system("clear");
            printf("Invalid selection. Please try again.\n\n");
            return 0;  // Return an error code for invalid selection
    }

    // Return a default value if no other case is executed
    return -1;
}
