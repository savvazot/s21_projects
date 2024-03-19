#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  bool numberNonBlank;
  bool markEnd;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintable;
} Flags;

void CatPrintNonPrintable(int *c);

void CatMarkEnd(int c);

void CatMarkTab(int *c);

void CatFile(FILE *file, Flags flags);
#endif