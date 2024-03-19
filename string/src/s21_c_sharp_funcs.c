#include "s21_string.h"

void *s21_to_upper(const char *str) {
  char *upper_str = s21_NULL;

  if (str) {
    s21_size_t str_len = s21_strlen(str);
    upper_str = (char *)calloc(str_len + 1, sizeof(char));

    if (upper_str) {
      for (s21_size_t i = 0; i < str_len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
          upper_str[i] = str[i] - 'a' + 'A';
        else
          upper_str[i] = str[i];
      }
      upper_str[str_len] = '\0';
    }
  }
  return upper_str;
}
void *s21_to_lower(const char *str) {
  char *lower_str = s21_NULL;

  if (str) {
    s21_size_t str_len = s21_strlen(str);
    lower_str = (char *)calloc(str_len + 1, sizeof(char));

    if (lower_str) {
      for (s21_size_t i = 0; i < str_len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
          lower_str[i] = str[i] - 'A' + 'a';
        else
          lower_str[i] = str[i];
      }
      lower_str[str_len] = '\0';
    }
  }
  return lower_str;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *inserted_str = s21_NULL;

  if (src && str) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);
    s21_size_t full_len = src_len + str_len;

    if (start_index <= src_len)
      inserted_str = (char *)calloc(full_len + 1, sizeof(char));

    if (inserted_str) {
      const char *src_ptr = src;
      char *ins_ptr = inserted_str;

      // copy first part
      while (src_ptr != src + start_index) *ins_ptr++ = *src_ptr++;

      // copy str
      const char *str_ptr = str;
      while (*str_ptr) *ins_ptr++ = *str_ptr++;

      // last part of src
      while (*src_ptr) *ins_ptr++ = *src_ptr++;
      *ins_ptr = '\0';
    }
  }
  return inserted_str;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *trim_str = s21_NULL;

  if (src && trim_chars) {
    const char *start = src;
    const char *end = src + s21_strlen(src) - 1;

    // find start and end, skipping trim chars
    while (*start && s21_strchr(trim_chars, *start) != s21_NULL) start++;
    while (end >= start && s21_strchr(trim_chars, *end) != s21_NULL) end--;

    s21_size_t trim_len = end - start + 1;
    if (trim_len > 0) {
      trim_str = (char *)calloc(trim_len + 1, sizeof(char));

      // fill trim_str
      if (trim_str) {
        char *trim_ptr = trim_str;
        while (start <= end) *trim_ptr++ = *start++;
        *trim_ptr = '\0';
      }

    } else {
      // return ""
      trim_str = (char *)calloc(1, sizeof(char));
      if (trim_str) {
        *trim_str = '\0';
      }
    }
  }
  return trim_str;
}