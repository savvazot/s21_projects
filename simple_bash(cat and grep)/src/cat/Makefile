CC=gcc
CFLAGS=-Wall -Wextra -Werror
all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o -o s21_cat 

test: test_files/tests.sh
	bash test_files/tests.sh

s21_cat.o: s21_cat.c s21_cat.h 
	$(CC) $(CFLAGS) s21_cat.c -c

clean:
	rm -rf *.o s21_cat

rebuild: clean all