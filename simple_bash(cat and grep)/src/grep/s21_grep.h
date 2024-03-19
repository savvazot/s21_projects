#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE
#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int regex_flag;
  bool invert;
  bool count;
  bool filesMatch;
  bool numberLine;
} Flags;

void GrepCount(FILE *file, Flags flags, regex_t *preg, char const *filename,
               int count_file);

void GrepFile(FILE *file, Flags flags, regex_t *preg, char const *filename,
              int count_file);

void Grep(int argc, char *argv[], Flags flags);
#endif