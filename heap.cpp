#include "heap.h"
#include <iostream>
#include <cassert>

using namespace std;

template <class T, class C>
void dump(const Heap<T, C>& heap)
{
	for (const auto& it : heap) {
		cout << it << " ";
	}
	cout << endl;
}

void max_heap()
{
	cout << "max_heap" << endl;

	Heap<int, std::greater<int>> heap;

	heap.push(10);
	heap.push(50);
	heap.push(5);
	heap.push(500);

	dump(heap);
	assert(heap.front() == 500);

	heap.pop();
	dump(heap);
	assert(heap.front() == 50);

	heap.pop();
	dump(heap);
	assert(heap.front() == 10);

	heap.pop();
	dump(heap);
	assert(heap.front() == 5);

	heap.pop();
	dump(heap);
}

void min_heap()
{
	cout << "min_heap" << endl;

	Heap<int, std::less<int>> heap;
	heap.push(10);
	heap.push(50);
	heap.push(5);
	heap.push(500);

	dump(heap);
	assert(heap.front() == 5);

	heap.pop();
	dump(heap);
	assert(heap.front() == 10);

	heap.pop();
	dump(heap);
	assert(heap.front() == 50);

	heap.pop();
	dump(heap);
	assert(heap.front() == 500);

	heap.pop();
	dump(heap);
}

int main()
{
	max_heap();
	min_heap();
	return 0;
}
