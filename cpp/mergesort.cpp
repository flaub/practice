#include "mergesort.hpp"

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void words() {
  std::vector<std::string> words;
  std::ifstream file("/usr/share/dict/words");
  std::string word;
  while (std::getline(file, word)) {
    words.push_back(word);
  }

  merge_sort(words.begin(), words.end());
  for (const std::string& word : words) {
    cout << word << endl;
  }
  cout << endl;
}

int main() {
  words();

  std::vector<int> test = {5, 2, 9, 43, 1, 4, 20, 99, 8};

  cout << "pre" << endl;
  for (const int& value : test) {
    cout << value << " ";
  }
  cout << endl;

  merge_sort(test.begin(), test.end());

  cout << "post" << endl;
  for (const int& value : test) {
    cout << value << " ";
  }
  cout << endl;

  merge_sort(test.begin(), test.end());

  cout << "sorted" << endl;
  for (const int& value : test) {
    cout << value << " ";
  }
  cout << endl;

  return 0;
}
