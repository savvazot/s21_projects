#include "s21_string.h"
s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  for (; str[len]; len++)
    ;
  return len;
}

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *new_str = (const unsigned char *)str;
  // When using void pointers in calculations, the behaviour is undefined.
  void *result = s21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (new_str[i] == (unsigned char)c) {
      result = (void *)(new_str + i);
      break;
    }
  }
  return result;
}

char *s21_strchr(const char *str, int c) {
  char *result = s21_NULL;
  for (s21_size_t i = 0; i <= s21_strlen(str); i++) {
    if (str[i] == c) {
      result = (char *)str + i;
      break;
    }
  }
  return result;
}

char *s21_strrchr(const char *str, int c) {
  char *result = s21_NULL;
  for (s21_size_t i = 0; i <= s21_strlen(str); i++) {
    if (str[i] == c) {
      result = (char *)str + i;
    }
  }
  return result;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  s21_size_t str1_len = s21_strlen(str1);
  s21_size_t str2_len = s21_strlen(str2);
  char *result = s21_NULL;
  int isFound = 0;
  for (s21_size_t i = 0; i < str1_len && !isFound; i++) {
    for (s21_size_t j = 0; j < str2_len; j++) {
      if (str1[i] == str2[j]) {
        result = (char *)str1 + i;
        isFound = 1;
        break;
      }
    }
  }
  return result;
}

char *s21_strstr(const char *haystack, const char *needle) {
  s21_size_t haystack_len = s21_strlen(haystack);
  s21_size_t needle_len = s21_strlen(needle);
  char *result = s21_NULL;
  if (haystack_len != 0) {
    for (s21_size_t i = 0; i <= haystack_len - needle_len; i++) {
      s21_size_t j = 0;
      for (; j < needle_len; j++) {
        if (haystack[i + j] != needle[j]) {
          break;
        }
      }
      if (j == needle_len) {
        result = (char *)haystack + i;
        break;
      }
    }
  }

  return result;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (*((unsigned char *)str1 + i) != *((unsigned char *)str2 + i)) {
      result = *((unsigned char *)str1 + i) - *((unsigned char *)str2 + i);
      break;
    }
  }
  return result > 0 ? 1 : result < 0 ? -1 : 0;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0') {
      result = str1[i] - str2[i];
      break;
    }
  }
  return result > 0 ? 1 : result < 0 ? -1 : 0;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++)
    *((unsigned char *)dest + i) = *((unsigned char *)src + i);
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) *((unsigned char *)str + i) = c;
  return str;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  for (; i < n && src[i] != '\0'; i++) dest[i] = src[i];
  for (; i < n; i++) dest[i] = '\0';
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t str1_len = s21_strlen(str1);
  s21_size_t str2_len = s21_strlen(str2);
  s21_size_t result = 0;
  int isFound = 0;
  for (s21_size_t i = 0; i <= str1_len && !isFound; i++) {
    for (s21_size_t j = 0; j <= str2_len; j++)
      if (str1[i] == str2[j]) {
        result = i;
        isFound = 1;
        break;
      }
  }
  return result;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t j = s21_strlen(dest);
  for (s21_size_t i = 0; i < n && src[i] != '\0'; i++) {
    dest[j++] = src[i];
  }
  dest[j] = '\0';
  return dest;
}

char *s21_strtok(char *str, const char *delim) {
  static char *pos = s21_NULL;
  char *start = s21_NULL;

  // разбиение с начала строки
  if (str != s21_NULL) {
    pos = str;
  }

  if (pos == s21_NULL || *pos == '\0')
    start = s21_NULL;

  else {
    // начало части, пропуская разделители
    while (*pos != '\0' && s21_strchr(delim, *pos) != s21_NULL) {
      pos++;
    }
    start = pos;

    // конец части, до первого разделителя или конца строки
    while (*pos != '\0' && s21_strchr(delim, *pos) == s21_NULL) {
      pos++;
    }
    if (*pos != '\0') {
      *pos = '\0';
      pos++;
    }
    // проверка на всю строку разделителей
    if (*start == '\0') start = s21_NULL;
  }
  return start;
}

char *s21_strerror(int errnum) {
  char *errors_list[] = {
#ifdef __APPLE__
      mac_errors
#elif __linux__
      linux_errors
#endif
  };

  // uncomment when sprintf is ready

  static char message[50];

  int is_known_error = errnum >= 0 && errnum < errors_size;
  if (!is_known_error) {
#ifdef __APPLE__
    sprintf(message, "Unknown error: %d", errnum);
    // s21_sprintf(message, "Unknown error: %d", errnum);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#elif __linux__
    sprintf(message, "Unknown error %d", errnum);
#endif
  }
  return is_known_error ? errors_list[errnum] : message;
}