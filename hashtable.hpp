#pragma once

#include "hash.hpp"

#include <tuple>
#include <vector>
#include <algorithm>

template <class K, class V, class HashFn = hash<K>>
class HashTable
{
public:
	typedef K key_type;
	typedef V mapped_type;
	typedef std::pair<K, V> value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;

public:
	HashTable()
		: m_buckets(100)
	{}

	iterator end()
	{
		return nullptr;
	}

	const_iterator end() const
	{
		return nullptr;
	}

	std::pair<iterator, bool> insert(const value_type& value)
	{
		resize(m_entries + 1);

		auto& bucket_ = m_buckets[bucket(value.first)];
		for (value_type& node : bucket_) {
			if (node.first == value.first) {
				return std::make_pair(&node, false);
			}
		}

		if (bucket_.empty()) {
			m_filled++;
		}

		bucket_.push_back(value);

		m_entries++;
		return std::make_pair(&bucket_.back(), true);
	}

	size_t erase(const key_type& key)
	{
		auto& bucket_ = m_buckets[bucket(key)];
		for (size_t i = 0; i < bucket_.size(); i++) {
			if (bucket_[i].first == key) {
 				size_t last = bucket_.size() - 1;
				std::swap(bucket_[i], bucket_[last]);
				bucket_.erase(bucket_.begin() + last);
				if (bucket_.empty()) {
					m_filled--;
				}
				m_entries--;
				return 1;
			}
		}

		return 0;
	}

	mapped_type& operator[](const key_type& key)
	{
		auto& bucket_ = m_buckets[bucket(key)];
		for (value_type& node : bucket_) {
			if (node.first == key) {
				return node.second;
			}
		}
		bucket_.emplace_back(key, mapped_type());
		return bucket_.back().second;
	}

	iterator find(const key_type& key)
	{
		auto& bucket_ = m_buckets[bucket(key)];
		for (value_type& node : bucket_) {
			if (node.first == key) {
				return &node;
			}
		}
		return end();
	}

	const_iterator find(const key_type& key) const
	{
		const auto& bucket_ = m_buckets[bucket(key)];
		for (const value_type& node : bucket_) {
			if (node.first == key) {
				return &node;
			}
		}
		return end();
	}

	double load_factor() const
	{
		return (double)m_entries / (double)m_buckets.size();
	}

	size_t size() const
	{
		return m_entries;
	}

	size_t bucket_count() const
	{
		return m_buckets.size();
	}

	size_t bucket_size(size_t n) const
	{
		return m_buckets[n].size();
	}

	size_t bucket(const key_type& key) const
	{
		return m_hasher(key) % bucket_count();
	}

	double avg_collisions() const
	{
		return (double)m_entries / (double)m_filled;
	}

private:
	void resize(size_t size)
	{
		if (load_factor() >= 0.75) {
			m_filled = 0;
			bucket_t tmp(m_buckets.size() * 2);
			for (const auto& bucket_ : m_buckets) {
				for (const value_type& node : bucket_) {
					auto& target = tmp[bucket(node.first, tmp.size())];
					if (target.empty()) {
						m_filled++;
					}
					target.push_back(node);
				}
			}
			m_buckets.swap(tmp);
		}
	}

	size_t bucket(const K& key, size_t size) const
	{
		return m_hasher(key) % size;
	}

private:
	typedef std::vector<value_type> nodes_t;
	typedef std::vector<nodes_t> bucket_t;
	bucket_t m_buckets;
	HashFn m_hasher;
	size_t m_entries = 0;
	size_t m_filled = 0;
};
