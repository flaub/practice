#include "hashtable.hpp"
#include "quicksort.hpp"

#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	HashTable<std::string, std::vector<std::string>> table;
	std::ifstream file("/usr/share/dict/words");
	std::string word;

	cout << "Phase 1" << endl;

	while (std::getline(file, word)) {
		std::string key = word;
		quick_sort(key.begin(), key.end());
		table[key].push_back(word);
	}

	file.clear();
	file.seekg(0, ios::beg);

	cout << "Phase 2" << endl;

	while (std::getline(file, word)) {
		std::string key = word;
		quick_sort(key.begin(), key.end());

		std::vector<std::string> anagrams;
		for (const auto& anagram : table[key]) {
			if (word != anagram) {
				anagrams.push_back(anagram);
			}
		}

		if (!anagrams.empty()) {
			cout << word << ":";
			for (const auto& anagram : anagrams) {
				cout << " " << anagram;
			}
			cout << endl;
		}
	}
}
