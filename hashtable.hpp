#pragma once

#include "hash.hpp"

#include <tuple>
#include <vector>
#include <algorithm>

template <class K, class V, class HashFn = hash<K>>
class HashTable
{
private:
	struct Node
	{
		Node(K key, V value)
			: key(key)
			, value(value)
		{}

		K key;
		V value;
	};

public:
	HashTable()
		: m_buckets(100)
	{
	}

	bool insert(const K& key, const V& value)
	{
		resize(m_entries + 1);

		auto& bucket = m_buckets[bucket_index(key)];
		for (const Node& node : bucket) {
			if (node.key == key) {
				return false;
			}
		}

		if (bucket.empty()) {
			m_filled++;
		}

		bucket.emplace_back(key, value);

		m_max_nodes = std::max(m_max_nodes, bucket.size());
		m_entries++;
		return true;
	}

	bool erase(const K& key)
	{
		auto& bucket = m_buckets[bucket_index(key)];
		for (size_t i = 0; i < bucket.size(); i++) {
			Node& node = bucket[i];
			if (node.key == key) {
				size_t last = bucket.size() - 1;
				std::swap(bucket[i], bucket[last]);
				bucket.erase(bucket.begin() + last);
				m_entries--;
				return true;
			}
		}

		return false;
	}

	bool lookup(const K& key, V& value) const
	{
		const auto& bucket = m_buckets[bucket_index(key)];
		for (const Node& node : bucket) {
			if (node.key == key) {
				value = node.value;
				return true;
			}
		}
		return false;
	}

	double load_factor() const
	{
		return (double)m_entries / (double)m_buckets.size();
	}

	size_t size() const
	{
		return m_entries;
	}

	size_t num_buckets() const
	{
		return m_buckets.size();
	}

	std::tuple<size_t, double> statistics() const
	{
		return std::make_tuple(m_max_nodes, (double)m_entries / (double)m_filled);
	}

private:
	void resize(size_t size)
	{
		if (load_factor() >= 0.75) {
			m_filled = 0;
			m_max_nodes = 0;
			bucket_t tmp(m_buckets.size() * 2);
			for (const auto& bucket : m_buckets) {
				for (const Node& node : bucket) {
					auto& target = tmp[bucket_index(node.key, tmp.size())];
					if (target.empty()) {
						m_filled++;
					}
					target.push_back(node);
					m_max_nodes = std::max(m_max_nodes, target.size());
				}
			}
			m_buckets.swap(tmp);
		}
	}

	size_t bucket_index(const K& key) const
	{
		return m_hasher(key) % m_buckets.size();
	}

	size_t bucket_index(const K& key, size_t size) const
	{
		return m_hasher(key) % size;
	}

private:
	typedef std::vector<Node> nodes_t;
	typedef std::vector<nodes_t> bucket_t;
	bucket_t m_buckets;
	HashFn m_hasher;
	size_t m_entries = 0;
	size_t m_max_nodes = 0;
	size_t m_filled = 0;
};
