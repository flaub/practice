#pragma once

#include <algorithm>

template <class RandomIt> void quick_sort(RandomIt first, RandomIt last) {
  size_t distance = std::distance(first, last);
  RandomIt pivot = first + (distance / 2);

  RandomIt it = first;
  RandomIt jt = last - 1;

  while (it <= jt) {
    while (*it < *pivot) {
      it++;
    }
    while (*jt > *pivot) {
      jt--;
    }
    if (it <= jt) {
      std::iter_swap(it, jt);
      it++;
      jt--;
    }
  }

  if (first < jt) {
    quick_sort(first, jt);
  }
  if (it < last) {
    quick_sort(it, last);
  }
}
