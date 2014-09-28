#pragma once

#include <vector>
#include <algorithm>

template <class ForwardIt>
std::vector<typename ForwardIt::value_type> merge_(ForwardIt first, ForwardIt middle, ForwardIt last)
{
	std::vector<typename ForwardIt::value_type> temp;
	temp.reserve(std::distance(first, last));
	
	ForwardIt left = first;
	ForwardIt right = middle;
	
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

template <class ForwardIt>
void merge_sort(ForwardIt first, ForwardIt last)
{
	size_t distance = std::distance(first, last);
	if (distance < 2) {
		return;
	}

	ForwardIt middle = first + (distance / 2);
	merge_sort(first, middle);
	merge_sort(middle, last);
	auto temp = merge_(first, middle, last);
	std::copy(temp.begin(), temp.end(), first);
}
