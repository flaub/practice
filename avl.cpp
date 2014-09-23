#include "avl.h"
#include <iostream>

using namespace std;

int main()
{
	AvlTree<int> tree;

	tree.insert(9);
	tree.insert(5);
	tree.insert(10);
	tree.insert(0);
	tree.insert(6);
	tree.insert(11);
	tree.insert(-1);
	tree.insert(1);
	tree.insert(2);

	cout << "Inserts" << endl;
	tree.display();
	tree.preorder([] (const int& key) {
		cout << key << endl;
	});
	cout << endl;

	tree.remove(10);

	cout << "Remove(10)" << endl;
	tree.display();
	tree.preorder([] (const int& key) {
		cout << key << endl;
	});

	return 0;
}
