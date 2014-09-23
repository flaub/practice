#!/usr/bin/env python

class Node(object):
	def __init__(self, value):
		self.value = value
		self.children = []

	def child(self, key):
		for kid in self.children:
			if kid.value == key:
				return kid
		return None

	def add(self, key):
		node = Node(key)
		self.children.append(node)
		return node

	def insert(self, value):
		node = self
		for i in range(len(value)):
			child = node.child(value[i])
			if child:
				node = child
			else:
				break

		while i < len(input):
			node = node.add(input[i])

def make_trie(input):
	root = Node('')
	for item in input:
		root.insert(item)
	return root

def has_common_prefix(input):
	root = Node('')

	for item in input:
		node = root
		for i in range(len(item)):
			if node.child(item[i]):
				return True
			break

		while i < len(item):
			node = node.add(item[i])
			i += 1

	return False

def main():
	assert has_common_prefix(['a', 'a'])
	assert has_common_prefix(['aa', 'ab'])
	assert has_common_prefix(['aa', 'bb', 'aba', 'abc', 'abbc'])
	assert not has_common_prefix(['a', 'b'])

if __name__ == '__main__':
	main()
