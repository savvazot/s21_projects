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

Flags CatReadFlags(int argc, char *argv[]) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};

  int currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);

  Flags flags = {false, false, false, false, false, false};

  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) {
    switch (currentFlag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.markEnd = true;
        flags.printNonPrintable = true;
        break;
      case 'v':
        flags.printNonPrintable = true;
        break;
      case 'E':
        flags.markEnd = true;
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.printNonPrintable = true;
        flags.tab = true;
        break;
      case 'T':
        flags.tab = true;
        break;
    }
  }
  return flags;
}

void CatPrintNonPrintable(int *c) {
  if (*c == '\n' || *c == '\t')
    ;
  else if (*c <= 31) {
    printf("^");
    *c += 64;
  } else if (*c == 127) {
    printf("^");
    *c = '?';
  } else if (*c >= 128 && *c < 128 + 32) {
    printf("M-^");
    *c -= 64;
  } else if (*c >= 128 + 32) {
    printf("M-");
    *c -= 128;
  }
}

void CatMarkEnd(int c) {
  if (c == '\n') {
    printf("$");
  }
}

void CatMarkTab(int *c) {
  if (*c == '\t') {
    *c = 'I';
    printf("^");
  }
}

void CatFile(FILE *file, Flags flags) {
  int c;
  char last = '\n';
  (void)flags;
  int countnumall = 1;
  int countblank = 1;
  int pust = 1;
  while ((c = getc(file)) != EOF) {
    if (last == '\n' && flags.numberAll) printf("%6d\t", countnumall++);

    if (flags.printNonPrintable) CatPrintNonPrintable(&c);
    if (flags.markEnd) CatMarkEnd(c);
    if (flags.tab) {
      CatMarkTab(&c);
    }

    if (flags.numberNonBlank && c != '\n' && last == '\n' && c != EOF &&
        countblank >= 1) {
      printf("%6d\t", countblank++);
    }

    if (pust == 1) {
      printf("%c", c);
    }

    if (c == '\n' && last == '\n' && flags.squeeze) {
      pust = 0;

    } else if (c != '\n' && last == '\n' && flags.squeeze) {
      printf("%c", c);
      pust = 1;
    }

    last = c;
  }
}
int main(int argc, char *argv[]) {
  Flags flags = CatReadFlags(argc, argv);

  char *filename = argv[1];
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      continue;
    }
    filename = argv[i];
    FILE *file;

    file = fopen(filename, "r+");
    if (file) {
      CatFile(file, flags);
    } else
      fprintf(stderr, "cat: %s: No such file or directory\n", filename);
  }

  return 0;
}