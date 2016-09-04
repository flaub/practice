#include "avl.hpp"
#include <iostream>

using namespace std;

template <class T>
void display_preorder(const T& tree)
{
	typedef typename T::key_type key_type;
	tree.preorder([] (const key_type* key) {
		cout << " ";
		if (key) {
			cout << *key;
		}
		else {
			cout << "#";
		}
	});
	cout << endl;
}

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
	display_preorder(tree);

	tree.remove(10);

	cout << "Remove(10)" << endl;
	tree.display();
	display_preorder(tree);

	std::string str = tree.serialize();
	auto copy = AvlTree<int>::deserialize(str);
	
	cout << "copy" << endl;
	copy.display();
	display_preorder(copy);

	return 0;
}
