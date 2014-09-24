#include "quicksort.hpp"
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

	quick_sort(test.begin(), test.end());

	cout << "post" << endl;
	for (const int& value : test) {
		cout << value << " ";
	}
	cout << endl;

	quick_sort(test.begin(), test.end());

	cout << "sorted" << endl;
	for (const int& value : test) {
		cout << value << " ";
	}
	cout << endl;

	return 0;
}

// example
// 
// partition (top)
//< 3  7  8  5 [2] 1  9  5  4 >
//  i                       j
// advance/rewind
//< 3  7  8  5 [2] 1  9  5  4 >
//  i              j
// swap
//< 1  7  8  5 [2] 3  9  5  4 >
//     i        j
// advance/rewind
//< 1  7  8  5 [2] 3  9  5  4 >
//     i        j
// swap
//< 1 [2] 8  5  7  3  9  5  4 >
//        i  j
// advance/rewind
//< 1 [2] 8  5  7  3  9  5  4 >
//  j     i
// partition (right)
//  1  2< 8  5  7 [3] 9  5  4 >
//        i                 j
// advance/rewind
//  1  2< 8  5  7 [3] 9  5  4 >
//        i        j
// swap
//  1  2<[3] 5  7  8  9  5  4 >
//           i  j
// advance/rewind
//  1  2<[3] 5  7  8  9  5  4 >
//        j  i  
// partition (right)
//  1  2  3< 5  7 [8] 9  5  4 >
//           i              j
// advance/rewind
//  1  2  3< 5  7 [8] 9  5  4 >
//                 i        j
// swap
//  1  2  3< 5  7  4  9  5 [8]>
//                    i  j
// advance/rewind
//  1  2  3< 5  7  4  9  5 [8]>
//                    i  j
// swap
//  1  2  3< 5  7  4  5  9 [8]>
//                    j  i
// partition (left) <-----------\
//  1  2  3< 5  7 [4] 5 >9  8   |
//           i        j         |
// advance/rewind               |
//  1  2  3< 5  7 [4] 5 >9  8   |
//           i     j            |
// swap                         |
//  1  2  3<[4] 7  5  5 >9  8   |
//              ij              |
// advance/rewind               |
//  1  2  3<[4] 7  5  5 >9  8   |
//           j  i               |
// partition (right)            |
//  1  2  3  4< 7 [5] 5 >9  8   |
//              i     j         |
// advance/rewind               |
//  1  2  3  4< 7 [5] 5 >9  8   |
//              i     j         |
// swap                         |
//  1  2  3  4< 5 [5] 7 >9  8   |
//                 ij           |
// advance/rewind               |
//  1  2  3  4< 5 [5] 7 >9  8   |
//              j     i         |
// partition (right) -----------/
//  1  2  3  4  5  5  7< 9 [8]>
//                       i  j
// advance/rewind
//  1  2  3  4  5  5  7< 9 [8]>
//                       i  j
// swap
//  1  2  3  4  5  5  7<[8] 9 >
//                       j  i
