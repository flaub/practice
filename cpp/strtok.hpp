#pragma once

char* my_strtok(char* str, const char* delim) {
  static char* last = nullptr;

  if (!str) {
    str = last;
  }

  if (!str) {
    return nullptr;
  }

  auto skip_leading = [&]() {
    for (const char* tmp = delim; *tmp; tmp++) {
      if (*str == *tmp) {
        str++;
        return true;
      }
    }
    return false;
  };

  while (skip_leading())
    ;

  char* token = str;

  for (; *str; str++) {
    for (const char* tmp = delim; *tmp; tmp++) {
      if (*str == *tmp) {
        last = str + 1;
        *str = '\0';
        return token;
      }
    }
  }

  last = nullptr;
  return token;
}
