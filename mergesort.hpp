#pragma once

#include <vector>
#include <algorithm>

template <class RandomIt>
std::vector<typename RandomIt::value_type> merge_(RandomIt first, RandomIt middle, RandomIt last)
{
	std::vector<typename RandomIt::value_type> temp;
	temp.reserve(std::distance(first, last));
	
	RandomIt left = first;
	RandomIt right = middle;
	
	while (left < middle && right < last) {
		if (*left < *right) {
			temp.push_back(*left++);
		}
		else {
			temp.push_back(*right++);
		}
	}

	while (left < middle) {
		temp.push_back(*left++);
	}

	while (right < last) {
		temp.push_back(*right++);
	}

	return temp;
}

template <class RandomIt>
void merge_sort_(RandomIt first, RandomIt last)
{
	size_t distance = std::distance(first, last);
	if (distance < 2) {
		return;
	}

	RandomIt middle = first + (distance / 2);
	merge_sort_(first, middle);
	merge_sort_(middle, last);
	auto temp = merge_(first, middle, last);
	std::copy(temp.begin(), temp.end(), first);
}

template <class RandomIt>
void merge_sort(RandomIt first, RandomIt last)
{
	merge_sort_(first, last);
}
