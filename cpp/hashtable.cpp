#include "hashtable.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

template <class T>
void contains(const T& table, const typename T::key_type& key, typename T::mapped_type expected_value)
{
	auto it = table.find(key);
	assert(it != table.end() && it->second == expected_value);
}

template <class T>
void missing(const T& table, const typename T::key_type& key)
{
	auto it = table.find(key);
	assert(it == table.end());
}

void words()
{
	HashTable<std::string, int> table;

	std::ifstream file("/usr/share/dict/words");
	std::string line;
	for (size_t i = 0; std::getline(file, line); i++) {
		table.insert(std::make_pair(line, i));
		if (i % 100 == 0) {
			printf("[%zu] load_factor: %f size: %zu buckets: %zu avg: %f\n", 
				i, 
				table.load_factor(), 
				table.size(), 
				table.bucket_count(),
				table.avg_collisions()
			);
		}
	}

	file.clear();
	file.seekg(0, ios::beg);

	for (size_t i = 0; std::getline(file, line); i++) {
		contains(table, line, i);
	}

	file.clear();
	file.seekg(0, ios::beg);

	for (size_t i = 0; std::getline(file, line); i++) {
		table.erase(line);
		if (i % 100 == 0) {
			printf("[%zu] load_factor: %f size: %zu buckets: %zu avg: %f\n", 
				i, 
				table.load_factor(), 
				table.size(), 
				table.bucket_count(),
				table.avg_collisions()
			);
		}
	}
}

int main()
{
	HashTable<std::string, int> table;
	
	table.insert(std::make_pair("a", 1));
	table.insert(std::make_pair("abcdefg", 2));

	missing(table, "b");
	contains(table, "a", 1);
	contains(table, "abcdefg", 2);

	assert(table.erase("b") == 0);
	assert(table.erase("a") == 1);
	missing(table, "a");

	words();

	return 0;
}
