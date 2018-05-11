#include "strtok.hpp"
#include <iostream>

int main() {
  const char* delim = " |";
  char input[100] = "A|bird came | down the walk";
  char* token = my_strtok(input, delim);
  while (token) {
    std::cout << "'" << token << "'" << std::endl;
    token = my_strtok(nullptr, delim);
  }
  return 0;
}
