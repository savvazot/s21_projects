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

Flags GrepReadFlags(int argc, char *argv[]) {
  Flags flags = {0, false, false, false, false};
  int currentFlag = currentFlag = getopt(argc, argv, "eivcln");
  for (; currentFlag != -1; currentFlag = getopt(argc, argv, "eivcln")) {
    switch (currentFlag) {
      case 'e':
        flags.regex_flag |= REG_EXTENDED;
        break;
      case 'i':
        flags.regex_flag |= REG_ICASE;
        break;
      case 'v':
        flags.invert = true;
        break;
      case 'c':
        flags.count = true;
        break;
      case 'l':
        flags.filesMatch = true;
        break;
      case 'n':
        flags.numberLine = true;
        break;
    }
  }
  return flags;
}

void GrepCount(FILE *file, Flags flags, regex_t *preg, char const *filename,
               int count_file) {
  (void)flags;
  (void)filename;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      ++count;
    }
  }
  if (count_file == 1) {
    printf("%d\n", count);
  } else
    printf("%s:%d\n", filename, count);

  free(line);
}

void GrepFile(FILE *file, Flags flags, regex_t *preg, char const *filename,
              int count_file) {
  (void)flags;
  (void)filename;
  char *line = 0;
  int filesod = 0;
  int count_line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    count_line++;
    if (flags.invert) {
      if (count_file == 1) {
        if (regexec(preg, line, 1, &match, 0)) {
          printf("%s", line);
        }
      } else {
        if (regexec(preg, line, 1, &match, 0)) {
          printf("%s:%s", filename, line);
        }
      }
    } else if (flags.filesMatch) {
      if (!regexec(preg, line, 1, &match, 0)) {
        filesod++;
      }
    } else if (flags.numberLine) {
      if (count_file == 1) {
        if (!regexec(preg, line, 1, &match, 0)) {
          printf("%d:%s", count_line, line);
        }
      } else {
        if (!regexec(preg, line, 1, &match, 0)) {
          printf("%s:%d:%s", filename, count_line, line);
        }
      }
    } else {
      if (count_file == 1) {
        if (!regexec(preg, line, 1, &match, 0)) {
          printf("%s", line);
        }
      } else {
        if (!regexec(preg, line, 1, &match, 0)) {
          printf("%s:%s", filename, line);
        }
      }
    }
  }
  free(line);
  if (flags.filesMatch && filesod > 0) {
    printf("%s\n", filename);
  }
}
void Grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  int count = 0;
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  for (; pattern != end && pattern[0][0] == '-'; ++pattern)
    ;
  if (pattern == end) {
    fprintf(stderr, "no pattern\n");
    exit(1);
  }
  if (regcomp(preg, *pattern, flags.regex_flag)) {
    fprintf(stderr, "failed ti compile regex\n");
    exit(1);
  }
  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') continue;
    ++count;
  }
  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') continue;
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    if (flags.count) {
      GrepCount(file, flags, preg, *filename, count);
    } else
      GrepFile(file, flags, preg, *filename, count);
    fclose(file);
  }
  regfree(preg);
}

int main(int argc, char *argv[]) {
  Flags flags = GrepReadFlags(argc, argv);
  Grep(argc, argv, flags);
  return 0;
}
