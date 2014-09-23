#include "hashtable.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

void words()
{
	HashTable<std::string, int> table;

	std::ifstream file("/usr/share/dict/words");
	std::string line;
	for (size_t i = 0; std::getline(file, line); i++) {
		table.insert(line, i);
		auto stats = table.statistics();
		if (i % 100 == 0) {
			printf("[%zu] load_factor: %f size: %zu buckets: %zu max: %zu avg: %f\n", 
				i, 
				table.load_factor(), 
				table.size(), 
				table.num_buckets(),
				std::get<0>(stats),
				std::get<1>(stats)
			);
		}
	}

	std::ifstream file2("/usr/share/dict/words");
	for (size_t i = 0; std::getline(file2, line); i++) {
		int value;
		assert(table.lookup(line, value) && value == i);
	}
}

int main()
{
	HashTable<std::string, int> table;
	
	table.insert("a", 1);
	table.insert("abcdefg", 2);

	int value;

	assert(!table.lookup("b", value));
	assert(table.lookup("a", value) && value == 1);
	assert(table.lookup("abcdefg", value) && value == 2);

	assert(!table.erase("b"));
	assert(table.erase("a"));
	assert(!table.lookup("a", value));

	words();

	return 0;
}
