#include <stdio.h>
#include <stdlib.h>
#include "spreadsheet.h"

void printMainMenu();

int main() {
  system("clear");
  int isTerminated = 0;
  while(!isTerminated) {
    printMainMenu();
    isTerminated = validateSelection();
  }
}

void printMainMenu() {
      printf("Spreadsheet Menu\n"
           "----------------\n"
           "1. Display Spreadsheet\n"
           "2. Display Histogram\n"
           "3. Set Sort Column\n"
           "4. Update Last Name\n"
           "5. Update Exam Grade\n"
           "6. Update Grade Mapping\n"
           "7. Delete Student\n"
           "8. Exit\n"
           "\n");
}


