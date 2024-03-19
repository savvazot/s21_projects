#include "s21_string.h"

#include <string.h>

int main() {
  char str[256];
  char str2[256];
  // char str3[50];
  // char str4[50];
  // s21_sprintf(str3, "hola %-.8u hoho", 5);
  // puts(str3);
  // sprintf(str4, "hola %-.8u hoho", 5);
  // puts(str4);

  s21_sprintf(str, "%f", 19.999995);
  sprintf(str2, "%f", 19.999995);
  puts(str);
  puts(str2);

  return 0;
}
