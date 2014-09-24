#include "mergesort.hpp"
#include <vector>
#include <iostream>

using namespace std;

int main()
{
	std::vector<int> test = {
		5, 2, 9, 43, 1, 4, 20, 99, 8
	};

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
