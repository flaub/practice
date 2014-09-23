#pragma once

#include <vector>
#include <string>

class Trie
{
private:
	struct Node
	{
		Node() = default;

		Node(char value) 
			: value(value)
		{}

		Node* add(char value)
		{
			children.emplace_back(value);
			return &children.back();
		}

		Node* child(char value) 
		{
			for (auto& child : children) {
				if (child.value == value) {
					return &child;
				}
			}
			return nullptr;
		}

		char value;
		std::vector<Node> children;
	};

public:
	bool add(const std::string& value)
	{
		bool found_prefix = false;
		Node* node = &m_root;
		for (const auto& ch : value) {
			Node* child = node->child(ch);
			if (!child) {
				node = node->add(ch);
			}
			else {
				node = child;
				found_prefix = true;
			}
		}
		return found_prefix;
	}

private:
	Node m_root;
};

bool has_common_prefix(const std::vector<std::string>& values)
{
	Trie trie;
	for (const auto& value : values) {
		if (trie.add(value)) {
			return true;
		}
	}
	return false;
}
