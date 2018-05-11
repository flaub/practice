#pragma once

#include <cmath>
#include <functional>
#include <vector>

template <class T, class Compare = std::less<T>> class Heap {
public:
  typedef std::vector<T> array_t;
  typedef typename array_t::iterator iterator;
  typedef typename array_t::const_iterator const_iterator;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::function<void(const T&)> visitor_f;

public:
  reference top() { return m_array[0]; }

  const_reference top() const { return m_array[0]; }

  void push(const T& value) {
    Compare fn = Compare();

    size_t i = m_array.size();
    m_array.push_back(value);

    while (i && fn(m_array[i], m_array[parent_(i)])) {
      std::swap(m_array[i], m_array[parent_(i)]);
      i = parent_(i);
    }
  }

  void pop() {
    if (m_array.empty()) {
      return;
    }
    if (m_array.size() < 2) {
      m_array.clear();
      return;
    }
    size_t last = m_array.size() - 1;
    std::swap(m_array[0], m_array[last]);
    m_array.erase(m_array.begin() + last);
    heapify();
  }

  const_iterator begin() const { return m_array.begin(); }

  const_iterator end() const { return m_array.end(); }

  void preorder(visitor_f visit) { preorder_(0, visit); }

  void inorder(visitor_f visit) { inorder_(0, visit); }

  void postorder(visitor_f visit) { postorder_(0, visit); }

private:
  size_t parent_(size_t i) const { return (i - 1) / 2; }

  size_t child_(size_t i, size_t side) const { return 2 * i + side; }

  void heapify() {
    Compare fn = Compare();

    for (size_t i = 0; i < m_array.size();) {
      size_t extreme = 0;
      size_t left = child_(i, LEFT);
      size_t right = child_(i, RIGHT);

      if (left < m_array.size()) {
        extreme = left;
      }

      if (right < m_array.size() && fn(m_array[right], m_array[left])) {
        extreme = right;
      }

      if (extreme && fn(m_array[extreme], m_array[i])) {
        std::swap(m_array[extreme], m_array[i]);
        i = extreme;
      } else {
        break;
      }
    }
  }

  void preorder_(size_t i, visitor_f visit) {
    if (i >= m_array.size()) {
      return;
    }
    visit(m_array[i]);
    preorder_(child_(i, LEFT), visit);
    preorder_(child_(i, RIGHT), visit);
  }

  void inorder_(size_t i, visitor_f visit) {
    if (i >= m_array.size()) {
      return;
    }
    inorder_(child_(i, LEFT), visit);
    visit(m_array[i]);
    inorder_(child_(i, RIGHT), visit);
  }

  void postorder_(size_t i, visitor_f visit) {
    if (i >= m_array.size()) {
      return;
    }
    postorder_(child_(i, LEFT), visit);
    postorder_(child_(i, RIGHT), visit);
    visit(m_array[i]);
  }

private:
  const size_t LEFT = 1;
  const size_t RIGHT = 2;
  std::vector<T> m_array;
};
