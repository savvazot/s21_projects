CC = gcc -Wall -Wextra -Werror -std=c11
SAN = -g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

SRCS = $(wildcard *.c)
SRCS := $(filter-out s21_string.c,$(SRCS))
OBJS = $(SRCS:.c=.o)

all: s21_string
	./s21_string 
#remove when finish!!!



s21_string: s21_string.o s21_string.a
	$(CC) $^ -o $@ 

%.o: %.c
	$(CC) -g -c $^ -o $@

# static lib

s21_string.a: $(OBJS)
	ar rcs $@ $^

# test

test: tests/test.c s21_string.a
	$(CC) $^ -lcheck -lm -lpthread -o tests/$@
	tests/./test

tests/test.c: tests/*.check
	checkmk clean_mode=1 $^ > $@

#gcov

gcov_report: tests/test.c s21_string.a
	gcc -fprofile-arcs -ftest-coverage -lcheck -lm -lpthread $^ -o $@
	./gcov_report
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out
	open out/index.html

clean:
	rm -rf s21_string *.o *.a tests/*.o tests/test tests/*.c gcov_report *.gcno *.gcda *.info out

rebuild: clean all

# check.h linux: -lcheck -lpthread -lrt -lsubunit

# clang-format -i *.c
# cppcheck --enable=all --suppress=missingIncludeSystem *.c 
