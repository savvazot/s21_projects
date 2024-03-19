#include "s21_string.h"
int s21_atoi(const char* str) {
  int result = 0;
  int sign = 1;

  // skip spaces
  while (*str == ' ') str++;

  if (*str == '-' || *str == '+') {
    if (*str == '-') sign = -1;
    str++;
  }

  while (s21_isdigit(*str)) {
    result = result * 10 + *str - '0';
    str++;
  }

  result *= sign;
  return result;
}