#include "trie.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	assert(has_common_prefix({"aaa", "aab", "abc"}));
	assert(!has_common_prefix({"aaa", "bbb", "ccc"}));
	return 0;
}
