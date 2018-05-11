#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>

template <class T> void ss_serialize(std::stringstream& ss, const T& value) {
  size_t len = sizeof(value);
  ss.write((char*)&len, sizeof(len));
  ss.write((char*)&value, sizeof(value));
}

template <> void ss_serialize(std::stringstream& ss, const std::string& value) {
  size_t len = value.size();
  ss.write((char*)&len, sizeof(value.size()));
  ss.write(value.data(), value.size());
}

template <class T> T ss_deserialize(std::stringstream& ss) {
  size_t len;
  ss.read((char*)&len, sizeof(len));
  T value;
  ss.read((char*)&value, len);
  return value;
}

template <> std::string ss_deserialize(std::stringstream& ss) {
  size_t len;
  ss.read((char*)&len, sizeof(len));
  std::vector<char> buf(len);
  ss.read(buf.data(), buf.size());
  return std::string(buf.data(), buf.size());
}

template <class KeyType> class AvlTree {
private:
  struct Node {
    Node(const KeyType& key) : key(key) {}

    KeyType key;
    Node* left = nullptr;
    Node* right = nullptr;
    size_t height = 1;
  };

  Node* m_root = nullptr;

  AvlTree(Node* root) : m_root(root) {}

public:
  typedef KeyType key_type;
  typedef std::function<void(const KeyType*)> visitor_f;

  AvlTree() = default;
  ~AvlTree() { clean(m_root); }

  static AvlTree<KeyType> deserialize(const std::string& str) {
    std::stringstream ss(str);
    return AvlTree<KeyType>(deserialize(ss));
  }

  static Node* deserialize(std::stringstream& ss) {
    auto type = ss.get();
    if (type == 0) {
      return nullptr;
    }

    KeyType value = ss_deserialize<KeyType>(ss);
    Node* node = new Node(value);
    node->left = deserialize(ss);
    node->right = deserialize(ss);
    return node;
  }

  std::string serialize() const {
    std::stringstream ss;
    preorder([&](const KeyType* key) {
      if (key) {
        ss.put(1);
        ss_serialize(ss, *key);
      } else {
        ss.put(0);
      }
    });
    return ss.str();
  }

  void insert(const KeyType& key) { m_root = insert_(m_root, key); }

  void remove(const KeyType& key) { m_root = remove_(m_root, key); }

  void preorder(visitor_f visit) const { preorder_(m_root, visit); }

  void inorder(visitor_f visit) const { inorder_(m_root, visit); }

  void postorder(visitor_f visit) const { postorder_(m_root, visit); }

  void bfs(visitor_f visit) {
    if (!m_root) {
      return;
    }

    std::queue<Node*> queue;
    queue.push(m_root);

    while (!queue.empty()) {
      auto node = queue.front();
      queue.pop();
      visit(node->key);
      if (node->left) {
        queue.push(node->left);
      }
      if (node->right) {
        queue.push(node->right);
      }
    }
  }

  void display() {
    display_(m_root, 1);
    std::cout << std::endl;
  }

private:
  void clean(Node* node) {
    if (!node) {
      return;
    }
    clean(node->left);
    clean(node->right);
    delete node;
  }

  void preorder_(Node* node, visitor_f visit) const {
    if (!node) {
      visit(nullptr);
      return;
    }
    visit(&node->key);
    preorder_(node->left, visit);
    preorder_(node->right, visit);
  }

  void inorder_(Node* node, visitor_f visit) const {
    if (!node) {
      visit(nullptr);
      return;
    }
    inorder_(node->left, visit);
    visit(&node->key);
    inorder_(node->right, visit);
  }

  void postorder_(Node* node, visitor_f visit) const {
    if (!node) {
      visit(nullptr);
      return;
    }
    postorder_(node->left, visit);
    postorder_(node->right, visit);
    visit(&node->key);
  }

  void display_(Node* node, int level) {
    if (!node) {
      return;
    }

    display_(node->right, level + 1);
    std::cout << std::endl;
    if (node == m_root) {
      std::cout << "Root -> ";
    }
    for (size_t i = 0; i < level && node != m_root; i++) {
      std::cout << "        ";
    }
    std::cout << node->key;
    display_(node->left, level + 1);
  }

  Node* insert_(Node* node, const KeyType& key) {
    if (!node) {
      return new Node(key);
    }

    if (key < node->key) {
      node->left = insert_(node->left, key);
    } else {
      node->right = insert_(node->right, key);
    }

    return balance_(node);
  }

  Node* remove_(Node* node, const KeyType& key) {
    if (!node) {
      return nullptr;
    }

    if (key < node->key) {
      node->left = remove_(node->left, key);
    } else if (key > node->key) {
      node->right = remove_(node->right, key);
    } else {
      if (!node->left || !node->right) {
        Node* temp = node->left ? node->left : node->right;
        if (!temp) {
          temp = node;
          node = nullptr;
        } else {
          *node = *temp;
        }
        delete temp;
      } else {
        Node* temp = min_node(node->right);
        node->key = temp->key;
        node->right = remove_(node->right, temp->key);
      }
    }

    return balance_(node);
  }

  Node* balance_(Node* node) {
    if (!node) {
      return nullptr;
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balance = balance_factor(node);
    if (balance > 1) {
      Node* child = node->left;
      if (balance_factor(child) < 0) {
        node->left = rotate_left(child);
      }
      return rotate_right(node);
    } else if (balance < -1) {
      Node* child = node->right;
      if (balance_factor(child) > 0) {
        node->right = rotate_right(child);
      }
      return rotate_left(node);
    }
    return node;
  }

  Node* rotate_left(Node* parent) {
    Node* temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;

    parent->height = std::max(height(parent->left), height(parent->right)) + 1;
    temp->height = std::max(height(temp->left), height(temp->right)) + 1;

    return temp;
  }

  Node* rotate_right(Node* parent) {
    Node* temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;

    parent->height = std::max(height(parent->left), height(parent->right)) + 1;
    temp->height = std::max(height(temp->left), height(temp->right)) + 1;

    return temp;
  }

  size_t height(Node* node) {
    if (node) {
      return node->height;
    }
    return 0;
  }

  int balance_factor(Node* node) {
    if (node) {
      return height(node->left) - height(node->right);
    }
    return 0;
  }

  Node* min_node(Node* node) {
    Node* cur = node;
    while (cur->left) {
      cur = cur->left;
    }
    return cur;
  }
};
