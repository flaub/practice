#include "heap.hpp"
#include <cassert>
#include <iostream>

using namespace std;

template <class T, class C> void dump(const Heap<T, C>& heap) {
  for (const auto& it : heap) {
    cout << it << " ";
  }
  cout << endl;
}

void max_heap() {
  cout << "max_heap" << endl;

  Heap<int, std::greater<int>> heap;

  heap.push(10);
  heap.push(50);
  heap.push(5);
  heap.push(5);
  heap.push(500);

  dump(heap);
  assert(heap.top() == 500);

  heap.pop();
  dump(heap);
  assert(heap.top() == 50);

  heap.pop();
  dump(heap);
  assert(heap.top() == 10);

  heap.pop();
  dump(heap);
  assert(heap.top() == 5);

  heap.pop();
  dump(heap);
  assert(heap.top() == 5);

  heap.pop();
  dump(heap);
}

void min_heap() {
  cout << "min_heap" << endl;

  Heap<int, std::less<int>> heap;
  heap.push(10);
  heap.push(50);
  heap.push(5);
  heap.push(5);
  heap.push(500);

  dump(heap);
  assert(heap.top() == 5);

  heap.pop();
  dump(heap);
  assert(heap.top() == 5);

  heap.pop();
  dump(heap);
  assert(heap.top() == 10);

  heap.pop();
  dump(heap);
  assert(heap.top() == 50);

  heap.pop();
  dump(heap);
  assert(heap.top() == 500);

  heap.pop();
  dump(heap);
}

int main() {
  max_heap();
  min_heap();
  return 0;
}
