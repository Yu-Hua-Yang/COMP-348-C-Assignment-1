CC = gcc
CFLAGS = -Wall -g

spreadsheet_app: main.c student.c spreadsheet.c menu.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f spreadsheet_app