#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

#include "s21_string.h"

typedef struct {
  char flag;
  int width;
  int precision;
  char length;
  char type;
  int spec_len;
} Format;

int spec_d(va_list va, char *str, Format *parseFormat);
int spec_s(va_list va, char *str, Format *parseFormat);
int spec_u(va_list va, char *str, Format *parseFormat);
int spec_f(va_list va, char *str, Format *parseFormat);
int spec_c(va_list va, char *str, Format *parseFormat);
char *copy_number(int is_negative, int length, Format *parseFormat, char *str,
                  char *ptr, int *count);

Format Parser(const char *format) {
  Format parseFormat = {0, 0, -1, 0, 0, 0};
  char width[5];
  char precision[5];
  char *precisionPtr = precision;
  char *widthPtr = width;
  const char *start = format;

  while (*format && *format != 'c' && *format != 'i' && *format != 'd' &&
         *format != 'f' && *format != 's' && *format != 'u' && *format != '%') {
    if (*format == '+' || *format == '-' || *format == ' ') {
      parseFormat.flag = *format;
      format++;
    }
    while (*format && *format != '.' && *format != 'l' && *format != 'h' &&
           *format != 'c' && *format != 'i' && *format != 'd' &&
           *format != 'f' && *format != 's' && *format != 'u' &&
           *format != '%') {
      if (isdigit(*format))
        *widthPtr++ = *format++;
      else
        format++;
    }
    *widthPtr = '\0';
    if (atoi(width) > 0) {
      parseFormat.width = atoi(width);
    }
    while (*format && *format != 'l' && *format != 'h' && *format != 'c' &&
           *format != 'i' && *format != 'd' && *format != 'f' &&
           *format != 's' && *format != 'u' && *format != '%') {
      if (isdigit(*format))
        *precisionPtr++ = *format++;
      else
        format++;
    }
    *precisionPtr = '\0';
    if (atoi(precision) > 0) {
      parseFormat.precision = atoi(precision);
    }
    if (*format == 'h' || *format == 'l') {
      parseFormat.length = *format;
      format++;
    } else
      break;
  }
  parseFormat.type = *format;
  parseFormat.spec_len =
      format - start;  // Вычисляем количество обработанных символов
  return parseFormat;
}

int s21_sprintf(char *str, const char *format, ...) {
  int result = 0;
  va_list va;
  va_start(va, format);
  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
      result++;
      continue;
    }
    format++;
    if (*format == '%') {
      *str++ = '%';
      result++;
    }
    Format parseFormat = Parser(format);
    format += parseFormat.spec_len;
    printf("flag %c width %d precision %d length %c type %c\n",
           parseFormat.flag, parseFormat.width, parseFormat.precision,
           parseFormat.length, parseFormat.type);
    format++;

    switch (parseFormat.type) {
      case 'c':
        result += spec_c(va, str, &parseFormat);
        break;
      case 'd':
      case 'i':
        result += spec_d(va, str, &parseFormat);
        break;
      case 'f':
        result += spec_f(va, str, &parseFormat);
        break;
      case 's':
        result += spec_s(va, str, &parseFormat);
        break;
      case 'u':
        result += spec_u(va, str, &parseFormat);
        break;
    }
    while (*str) {
      str++;
      // result++;
    }
  }

  va_end(va);
  *str = '\0';
  return result;
}

int spec_d(va_list va, char *str, Format *parseFormat) {
  int count = 0;
  int arg_value = 0;
  if (parseFormat->length == 'h')
    arg_value = (short)va_arg(va, int);
  else if (parseFormat->length == 'l')
    arg_value = (long)va_arg(va, long);
  else
    arg_value = va_arg(va, int);
  int is_negative = arg_value < 0 ? 1 : 0;

  char buffer[256];
  s21_memset(buffer, 0, sizeof(buffer));
  if (arg_value == INT_MIN) {
    s21_strncpy(buffer, "-2147483648", s21_strlen("-2147483648"));
  } else
    s21_itoa(arg_value, buffer, 10);
  char *ptr = buffer;

  //
  int length = 0;
  while (buffer[length]) length++;

  // Handle the width
  if (parseFormat->flag == '-')
    str = copy_number(is_negative, length, parseFormat, str, ptr, &count);

  if (parseFormat->width > length) {
    parseFormat->flag == '+' && arg_value >= 0 ? length++ : 0;
    parseFormat->flag == ' ' && arg_value >= 0 ? length++ : 0;
    if (parseFormat->precision != -1) {
      if (length <= parseFormat->precision)
        parseFormat->width -= parseFormat->precision - length + is_negative;
      else
        parseFormat->width -= parseFormat->precision - length + 1 + 1;
    }
    for (int i = 0; i < parseFormat->width - length; i++) {
      *str++ = ' ';
      count++;
    }
  }

  // Handle the '+' and ' ' flags
  if (parseFormat->flag == '+' && arg_value >= 0) {
    *str++ = '+';
    count++;
  } else if (parseFormat->flag == ' ' && arg_value >= 0) {
    *str++ = ' ';
    count++;
  }

  // Copy the number

  if (parseFormat->flag != '-') {
    copy_number(is_negative, length, parseFormat, str, ptr, &count);
  }
  return count;
}
int spec_s(va_list va, char *str, Format *parseFormat) {
  char *arg_str = va_arg(va, char *);
  int count = 0;
  s21_size_t length = s21_strlen(arg_str);
  int effective_length =
      parseFormat->precision != -1 && parseFormat->precision < (int)length
          ? parseFormat->precision
          : (int)length;
  if (parseFormat->flag == '-' && parseFormat->width > 0) {
    while (*arg_str) {
      if (parseFormat->precision != -1 && parseFormat->precision == count)
        break;
      *str++ = *arg_str++;
      count++;
    }
    for (int i = count; i < parseFormat->width; i++) {
      *str++ = ' ';
      count++;
    }
  }

  else if (parseFormat->width > effective_length)
    for (int i = 0; i < parseFormat->width - effective_length; i++) {
      *str++ = ' ';
      count++;
    }

  while (*arg_str) {
    if (parseFormat->precision != -1 && parseFormat->precision == count) break;
    if (parseFormat->width && parseFormat->precision != -1 &&
        parseFormat->width == count && (int)length != effective_length)
      break;
    *str++ = *arg_str++;
    count++;
  }
  *str = '\0';
  return count;
}
int spec_u(va_list va, char *str, Format *parseFormat) {
  int count = 0;
  unsigned int arg_value = va_arg(va, unsigned int);
  char buffer[256];
  s21_uitoa(arg_value, buffer);
  int is_negative = 0;
  char *ptr = buffer;

  //
  int length = 0;
  while (buffer[length]) length++;

  // Handle the width
  if (parseFormat->flag == '-')
    str = copy_number(is_negative, length, parseFormat, str, ptr, &count);

  if (parseFormat->width > length) {
    for (int i = 0; i < parseFormat->width - length; i++) {
      *str++ = ' ';
      count++;
    }
  }

  // Copy the number
  if (parseFormat->flag != '-')
    copy_number(is_negative, length, parseFormat, str, ptr, &count);
  return count;
}
int spec_f(va_list va, char *str, Format *parseFormat) {
  int count = 0;
  double f;
  char f_char[256];
  char *f_char_ptr = f_char;
  int check_plus = 0;
  f = va_arg(va, double);
  if (parseFormat->precision == -1) parseFormat->precision = 6;
  s21_ftoa(f, f_char, parseFormat->precision);
  parseFormat->width = parseFormat->width - s21_strlen(f_char);
  if (parseFormat->flag == '+' && parseFormat->width < 1) {
    if (f >= 0) {
      *str++ = '+';
      check_plus = 1;
      parseFormat->width--;
    } else
      parseFormat->width--;
  } else if (parseFormat->flag == '+' && parseFormat->width >= 1) {
    if (f >= 0) {
      check_plus = 1;
      parseFormat->width--;
    } else
      parseFormat->width--;
  } else if (parseFormat->flag == ' ') {
    if (f >= 0) {
      *str++ = ' ';
      count++;
      parseFormat->width--;
    } else
      parseFormat->width--;
  }
  if (parseFormat->flag == '-' && parseFormat->width > 0) {
    for (s21_size_t i = 0; i < s21_strlen(f_char); i++) {
      *str++ = *f_char_ptr;
      f_char_ptr++;
      count++;
    }

    for (int i = 0; i < parseFormat->width; i++) {
      *str++ = ' ';
      count++;
    }
  } else if (parseFormat->width >= 0) {
    for (int i = 0; i < parseFormat->width + 1; i++) {
      *str++ = ' ';
      count++;
    }
    if (check_plus == 1 && f > 0 && parseFormat->flag == '+') {
      *str++ = '+';
      count++;

    } else if (parseFormat->flag == ' ') {
      *str++ = ' ';
      count++;
    }
    for (s21_size_t i = 0; i < s21_strlen(f_char); i++) {
      *str++ = *f_char_ptr;
      f_char_ptr++;
      count++;
    }

  } else {
    while (*f_char_ptr) {
      *str++ = *f_char_ptr;
      f_char_ptr++;
      count++;
    }
  }
  return count;
}
int spec_c(va_list va, char *str, Format *parseFormat) {
  int count = 0;
  char c;
  c = (char)va_arg(va, int);
  if (parseFormat->flag == '-' && parseFormat->width > 0) {
    *str++ = c;
    count++;
    for (int i = 0; i < (parseFormat->width - 1); i++) {
      *str++ = ' ';
      count++;
    }
  } else if (parseFormat->width > 0) {
    for (int i = 0; i < (parseFormat->width - 1); i++) {
      *str++ = ' ';
      count++;
    }
    *str++ = c;
    count++;
  } else {
    *str++ = c;
    count++;
  }
  return count;
}
char *copy_number(int is_negative, int length, Format *parseFormat, char *str,
                  char *ptr, int *count) {
  int pres = 0;

  if (is_negative) {
    *str++ = '-';
    *count += 1;
  }

  if (parseFormat->precision) {
    while (pres < parseFormat->precision - length + is_negative) {
      *str++ = '0';
      pres++;
      *count += 1;
    }
  }

  while (*ptr) {
    if (*ptr == '-') ptr++;
    *str++ = *ptr++;
    *count += 1;
  }
  (void)count;
  return str;
}
