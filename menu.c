#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

void displayMenu() {
    system("clear");  // Use "cls" for Windows
    printf("1. Display Spreadsheet\n");
    printf("2. Display Histogram\n");
    printf("3. Change Sort Order\n");
    printf("4. Update Student Name\n");
    printf("5. Update Student Grade\n");
    printf("6. Change Grade Scheme\n");
    printf("7. Remove Student\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

void changeSortOrder(Spreadsheet* sheet) {
    printf("Select sort column:\n");
    printf("0: Student ID\n");
    printf("1: Last Name\n");
    printf("2: Exam Grade (Descending)\n");
    printf("3: Total Grade (Descending)\n");
    printf("Enter your choice: ");
    scanf("%d", &sheet->sortColumn);
    sortSpreadsheet(sheet);
}

void handleMenuChoice(Spreadsheet* sheet) {
    int choice;
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            displaySpreadsheet(sheet);
            break;
        case 2:
            displayHistogram(sheet);
            break;
        case 3:
            changeSortOrder(sheet);
            break;
        case 4:
        {
            int studentId;
            char newLastName[MAX_NAME_LENGTH];
            printf("Enter student ID: ");
            scanf("%d", &studentId);
            printf("Enter new last name: ");
            scanf("%s", newLastName);
            updateStudentName(sheet, studentId, newLastName);
        }
        break;
        case 5:
            {
                int studentId; 
                int examGrade;
                printf("Enter student ID: ");
                scanf("%d", &studentId);
                printf("Enter new exam grade: (0-40): ");
                scanf("%d", &examGrade);
                updateStudentGrade(sheet, studentId, examGrade);
            }
            break;
        case 6:
            {
                GradeScheme newScheme;
                printf("Enter new grade boundaries:\n");
                printf("A: (>= ): ");
                scanf("%d", &newScheme.A);
                printf("B (>= ): ");
                scanf("%d", &newScheme.B);
                printf("C (>= ): ");
                scanf("%d", &newScheme.C);
                printf("D (>= ): ");
                scanf("%d", &newScheme.D);
                changeGradeScheme(sheet, newScheme);
            }
            break;
        case 7:
            {
                int studentId;
                printf("Enter student ID to remove: ");
                scanf("%d", &studentId);
                removeStudent(sheet, studentId);
            }
            break;
        case 8:
            printf("Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
    }

    printf("Press 'c' to continue...");
    while(getchar() != 'c');
}