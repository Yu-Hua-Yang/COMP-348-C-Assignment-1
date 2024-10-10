#include <stdio.h>
#include <stdlib.h>
#include "./helper_function/print.h"
#include "./helper_function/spreadsheet.h"
int main() {
  system("clear");
  int isTerminated = 0;
  while(!isTerminated) {
    printMainMenu();
    isTerminated = validateSelection();
  }
}
