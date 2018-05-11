#include "bloom_filter.hpp"
#include "hash.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

struct bloom_stats {
  void score(bool actual, bool expected) {
    if (expected) {
      if (actual) {
        true_positives++;
      } else {
        false_negatives++;
      }
    } else {
      if (actual) {
        false_positives++;
      } else {
        true_negatives++;
      }
    }
  }

  void analyze(double error_rate) {
    double false_positive_rate =
        (double)false_positives / (false_positives + true_negatives);

    printf("True positives:      %7zu\n", true_positives);
    printf("True negatives:      %7zu\n", true_negatives);
    printf("False positives:     %7zu\n", false_positives);
    printf("False negatives:     %7zu\n", false_negatives);
    printf("False positive rate:  %.4f\n", false_positive_rate);

    assert(false_negatives == 0);
    assert(false_positive_rate < error_rate);
  }

  size_t true_positives = 0;
  size_t true_negatives = 0;
  size_t false_positives = 0;
  size_t false_negatives = 0;
};

void test_basic() {
  printf("test_basic>\n");
  const size_t CAPACITY = 100;
  const double ERROR_RATE = 0.01;

  bloom_filter<2> bf(CAPACITY, ERROR_RATE);
  for (size_t i = 0; i < (CAPACITY - CAPACITY * 0.10); i++) {
    bf.add(prime_hasher(i));
  }

  bloom_stats stats;
  for (size_t i = 0; i < CAPACITY * 2; i++) {
    auto count = bf.lookup(prime_hasher(i));
    // printf("[%zu]: %zu\n", i, count);
    stats.score(count, i < (CAPACITY - CAPACITY * 0.10));
  }

  stats.analyze(ERROR_RATE);
  printf("\n");
}

void test_words() {
  printf("test_words>\n");
  const size_t CAPACITY = 100000;
  const double ERROR_RATE = 0.05;

  bloom_filter<2> bf(CAPACITY, ERROR_RATE);

  std::ifstream file("/usr/share/dict/words");
  std::string line;
  size_t count_added = 0;
  for (size_t i = 0; std::getline(file, line) && i < CAPACITY; i++) {
    if (i % 2 == 0) {
      bf.add(string_hasher(line));
      count_added++;
    }
  }

  file.clear();
  file.seekg(0);

  bloom_stats stats;
  size_t count_checked = 0;
  for (size_t i = 0; std::getline(file, line) && i < CAPACITY; i++) {
    auto count = bf.lookup(string_hasher(line));
    stats.score(count, (i % 2 == 0));
    count_checked++;
  }

  printf("Elements added:       %6zu\n", count_added);
  printf("Elements checked:     %6zu\n", count_checked);

  stats.analyze(ERROR_RATE);
  printf("\n");
}

int main() {
  test_basic();
  test_words();
  return 0;
}
