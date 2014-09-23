CFLAGS = -Wall -Werror -g
CXXFLAGS = -std=c++11 

BINS = \
	bin/avl \
	bin/heap \
	bin/trie

all: bin $(BINS)

bin:
	mkdir bin

bin/avl: avl.cpp avl.h
	$(CXX) $(CXXFLAGS) $(CFLAGS) avl.cpp -o $@

bin/heap: heap.cpp heap.h
	$(CXX) $(CXXFLAGS) $(CFLAGS) heap.cpp -o $@

bin/trie: trie.cpp trie.h
	$(CXX) $(CXXFLAGS) $(CFLAGS) trie.cpp -o $@

clean:
	rm -rf bin
