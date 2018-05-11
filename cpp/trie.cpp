#include "trie.hpp"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
  assert(has_common_prefix({"aaa", "aab", "abc"}));
  assert(!has_common_prefix({"aaa", "bbb", "ccc"}));
  return 0;
}
