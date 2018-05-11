#include <iostream>
#include <vector>

bool has_triplet(const std::vector<int>& integers) {
  for (int i = 0; i < integers.size(); i++) {
    for (int j = 0; j < integers.size(); j++) {
      for (int k = 0; k < integers.size(); k++) {
        if (i == j || i == k || j == k) {
          continue;
        }
        int a = integers[i];
        int b = integers[j];
        int c = integers[k];
        if ((a * a + b * b) == c * c)
          return true;
      }
    }
  }
  return false;
}

int main() {
  int num_testcases;
  std::cin >> num_testcases;
  for (int i = 0; i < num_testcases; i++) {
    int size;
    std::cin >> size;
    std::vector<int> integers;
    for (int j = 0; j < size; j++) {
      int value;
      std::cin >> value;
      integers.push_back(value);
    }
    if (has_triplet(integers)) {
      std::cout << "Yes" << std::endl;
    } else {
      std::cout << "No" << std::endl;
    }
  }

  return 0;
}
