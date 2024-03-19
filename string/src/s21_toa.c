#include <limits.h>
#include <math.h>

char* s21_itoa(int num, char* str, int base) {
  int i = 0;
  int is_negative = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  if (num < 0 && base == 10) {
    is_negative = 1;
    num = -num;
    if (num == INT_MIN) {
      num = INT_MAX;
    }
  }

  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  if (is_negative) {
    str[i++] = '-';
  }

  str[i] = '\0';

  // Reverse
  int start = 0;
  int end = i - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }

  return str;
}

char* s21_uitoa(unsigned int num, char* str) {
  char* start = str;
  if (num == 0) {
    *str++ = '0';
  } else {
    while (num != 0) {
      *str++ = '0' + num % 10;
      num = num / 10;
    }
  }
  char* end = str - 1;
  while (start < end) {
    char temp = *start;
    *start++ = *end;
    *end-- = temp;
  }
  *str = '\0';
  return start;
}

void s21_ftoa(long double value, char* str, int precision) {
  char integer_buf[150] = "";
  char fractional_buf[150] = "";
  long double integer_part;
  long double fractional_part;

  long double exp = powl(10, precision);

  // разделение на целую и дробную части
  if (!precision && (modfl(value, &integer_part) == 0.5)) {
    fractional_part = 0.0;
    integer_part += (int)integer_part % 2;
  } else {
    fractional_part = modfl(value, &integer_part);
    if (!precision) {
      integer_part = roundl(value);
    } else {
      integer_part += roundl(fractional_part * exp) / exp;
    }
  }

  // целая часть
  int i = 0;
  if (integer_part < 1) {
    *str++ = '0';
  }
  for (; integer_part >= 1; i++) {
    integer_buf[i] = (int)fmodl(integer_part, 10) + 48;
    integer_part /= 10;
  }
  i--;
  for (; i >= 0; --i) {
    *str++ = integer_buf[i];
  }

  // дробная часть
  if (precision) {
    *str++ = '.';
    // printf("||%s\n", str);
    fractional_part = roundl(fractional_part * exp);
    i = 0;
    for (; i < precision; i++) {
      fractional_buf[precision - i - 1] = (int)fmodl(fractional_part, 10) + 48;
      fractional_part /= 10;
    }
    // добавление нулей
    for (; i >= 0; i--) *str++ = fractional_buf[precision - i];
  }
  *str = 0;
}