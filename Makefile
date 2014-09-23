CFLAGS = -Wall -Werror -g
CXXFLAGS = -std=c++11 

BINS = \
	bin/avl \
	bin/heap \
	bin/trie \
	bin/hashtable \
	bin/bloom_filter

all: bin $(BINS)

bin:
	mkdir bin

bin/avl: avl.cpp avl.hpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) avl.cpp -o $@

bin/heap: heap.cpp heap.hpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) heap.cpp -o $@

bin/trie: trie.cpp trie.hpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) trie.cpp -o $@

bin/hashtable: hashtable.cpp hashtable.hpp hash.hpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) hashtable.cpp -o $@

bin/bloom_filter: bloom_filter.cpp bloom_filter.hpp packed_vector.hpp hash.hpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) bloom_filter.cpp -o $@

clean:
	rm -rf bin
