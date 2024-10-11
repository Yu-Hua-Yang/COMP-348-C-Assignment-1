#include <stdio.h>
#include "spreadsheet.h"
#include "menu.h"
#include "student.h"

int main() {
    Spreadsheet sheet;
    initSpreadsheet(&sheet);
    if (loadStudents(&sheet, "students.txt") != 0) {
        fprintf(stderr, "Failed to load students.\n");
        return 1;
    }
    while (1) {
        displayMenu();
        handleMenuChoice(&sheet);
    }
    return 0;
}