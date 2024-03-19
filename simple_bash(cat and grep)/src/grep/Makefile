CC=gcc
CFLAGS=-Wall -Wextra -Werror
all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(CFLAGS) s21_grep.o -o s21_grep 

test: test_files/tests.sh
	bash test_files/tests.sh

s21_grep.o: s21_grep.c s21_grep.h 
	$(CC) $(CFLAGS) s21_grep.c -c

clean:
	rm -rf *.o s21_grep

rebuild: clean all