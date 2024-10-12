#include <stdio.h>
#include <stdlib.h>
#include "spreadsheet.h"
#include "data.h"
#include "student.h"


/**
 * Clears the input buffer by consuming all characters until a newline or EOF.
 *
 * This function is used to clear the input buffer after a scanf call to prevent
 * extraneous characters from affecting future input operations.
 */
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/**
 * Prompts the user to enter a valid character to continue.
 *
 * The function loops until a valid character ('c' or 'C') is entered.
 * If the input is invalid, the function will loop until a valid input is entered.
 * After a valid input is received, the function clears the console and returns 0.
 *
 * @return 0 if the user enters a valid character ('c' or 'C'), 1 otherwise.
 */
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

/**
 * Validates user input for the spreadsheet menu and performs the corresponding action.
 *
 * Prompts the user to enter a menu selection and validates the input to ensure it is a valid menu option (1-8).
 * If the input is invalid, the function will loop until a valid input is entered.
 * The function performs the corresponding action based on the user's selection.
 *
 * @return 1 if the user selects the exit option (8), 0 otherwise.
 */
int validateSelection() {
    int selectionNumber;
    printf("Selection: ");
    scanf("%d", &selectionNumber);
    system("clear");
    Student* students = readFile();

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
            removeStudent(students);
            waitForContinue();
            return 0;
        case 8:
            printf("Goodbye and thanks for using our spreadsheet app\n"
                    "\n");
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
