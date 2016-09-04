#pragma once

#include "hash.hpp"

#include <vector>
#include <algorithm>

template <class K, class V, class HashFn = hash<K>>
class HashTable
{
public:
	typedef K key_type;
	typedef V mapped_type;
	typedef std::pair<K, V> value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;

private:
	typedef std::vector<value_type> bucket_t;
	typedef std::vector<bucket_t> buckets_t;

public:
	HashTable()
		: m_buckets(64)
	{}

	pointer end()
	{
		return nullptr;
	}

	const_pointer end() const
	{
		return nullptr;
	}

	std::pair<pointer, bool> insert(const value_type& value)
	{
		resize(m_entries + 1);

		auto result = find_(value.first);
		auto bp = result.first;
		auto it = result.second;

		if (it != end()) {
			return std::make_pair(it, false);
		}

		if (bp->empty()) {
			m_used++;
		}

		bp->push_back(value);
		m_entries++;

		return std::make_pair(&bp->back(), true);
	}

	size_t erase(const key_type& key)
	{
		resize(m_entries - 1);

		auto result = find_(key);
		auto bp = result.first;
		auto it = result.second;

		if (it == end()) {
			return 0;
		}

		auto last = bp->begin() + (bp->size() - 1);
		std::iter_swap(it, last);
		bp->erase(last);
		if (bp->empty()) {
			m_used--;
		}
		m_entries--;
		return 1;
	}

	mapped_type& operator[](const key_type& key)
	{
		resize(m_entries + 1);

		auto result = find_(key);
		auto bp = result.first;
		auto it = result.second;

		if (it != end()) {
			return it->second;
		}

		bp->emplace_back(key, mapped_type());
		m_entries++;

		return bp->back().second;
	}

	pointer find(const key_type& key)
	{
		return find_(key).second;
	}

	const_pointer find(const key_type& key) const
	{
		return find_(key).second;
	}

	double load_factor() const
	{
		return load_factor_(m_entries);
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
		return (double)m_entries / (double)m_used;
	}

private:
	double load_factor_(size_t entries) const
	{
		return (double)entries / (double)m_buckets.size();
	}

	std::pair<bucket_t*, pointer> find_(const key_type& key)
	{
		auto& bucket_ = m_buckets[bucket(key)];
		for (auto& node : bucket_) {
			if (node.first == key) {
				return std::make_pair(&bucket_, &node);
			}
		}
		return std::make_pair(&bucket_, end());
	}

	std::pair<const bucket_t*, const_pointer> find_(const key_type& key) const
	{
		const auto& bucket_ = m_buckets[bucket(key)];
		for (const auto& node : bucket_) {
			if (node.first == key) {
				return std::make_pair(&bucket_, &node);
			}
		}
		return std::make_pair(&bucket_, end());
	}

	void resize(size_t size)
	{
		if (load_factor_(size) >= 0.75) {
			rehash(m_buckets.size() * 2);
		}
		else if (m_entries > 64 && load_factor_(size) < 0.25) {
			rehash(m_buckets.size() / 2);
		}
	}

	void rehash(size_t size)
	{
		m_used = 0;
		buckets_t tmp(size);
		for (const auto& bucket_ : m_buckets) {
			for (const auto& node : bucket_) {
				auto& target = tmp[bucket(node.first, tmp.size())];
				if (target.empty()) {
					m_used++;
				}
				target.push_back(node);
			}
		}
		m_buckets.swap(tmp);
	}

	size_t bucket(const K& key, size_t size) const
	{
		return m_hasher(key) % size;
	}

private:
	buckets_t m_buckets;
	HashFn m_hasher;
	size_t m_entries = 0;
	size_t m_used = 0;
};
