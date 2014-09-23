#pragma once

#include <vector>
#include <thread>

namespace packed
{
namespace detail
{
template <class T, class C>
struct base_accessor
{
	typedef typename T::element_type element_type;
	typedef typename T::value_type value_type;

	base_accessor(C& container, size_t pos)
		: container(container)
		, bit_offset(pos * T::value_width)
		, index(bit_offset / T::element_width)
		, start(bit_offset % T::element_width)
		, mask((1ULL << T::value_width) - 1)
	{}

	C& container;
	const size_t bit_offset;
	const size_t index;
	const size_t start;
	const element_type mask;
};

template <class T, class C, bool Pow2>
struct accessor : public base_accessor<T, C>
{
	accessor(C& container, size_t pos)
		: base_accessor<T, C>(container, pos)
		, available(T::element_width - this->start)
	{}

	operator typename T::value_type() const
	{
		auto element = this->container[this->index];
		if (T::value_width <= this->available) {
			auto raw = (element >> this->start) & this->mask;
			return typename T::value_type(raw);
		}
		auto next = this->container[this->index+1];
		auto low = element >> this->start;
		auto high = next << (T::element_width - this->start);
		auto raw = low ^ ((low ^ high) & (this->mask >> this->available << this->available));
		return typename T::value_type(raw);
	}

	void operator=(const typename T::value_type& value) const
	{
		typedef typename T::element_type element_type;
		auto native_value = static_cast<element_type>(value);
		auto probe = &this->container[this->index];
		if (T::value_width <= this->available) {
			auto append = (native_value << this->start);
			bool ok = false;
			do {
				volatile auto old_value = *probe;
				auto new_value = (old_value & ~(this->mask << this->start)) | append;
				ok = __sync_bool_compare_and_swap(probe, old_value, new_value);
			} while (!ok);
			return;
		}
		auto next = &this->container[this->index+1];
		auto low = native_value << this->start;
		auto high = native_value >> this->available;
		bool ok = false;
		do {
			volatile auto old_value = *probe;
			auto new_value = (old_value & ~(this->mask << this->start)) | low;
			ok = __sync_bool_compare_and_swap(probe, old_value, new_value);
		} while (!ok);
		do {
			volatile auto old_value = *next;
			auto new_value = (old_value & ~(this->mask >> (T::element_width - this->start))) | high;
			ok = __sync_bool_compare_and_swap(next, old_value, new_value);
		} while (!ok);
	}

	// TODO: safe_increment()

	const size_t available;
};

template <class T, class C>
struct accessor<T, C, true> : public base_accessor<T, C>
{
	inline
	accessor(C& container, size_t pos)
		: base_accessor<T, C>(container, pos)
	{}

	inline
	operator typename T::value_type() const
	{
		auto element = this->container[this->index];
		auto raw = (element >> this->start) & this->mask;
		return typename T::value_type(raw);
	}

	inline
	void operator=(const typename T::value_type& value) const
	{
		typedef typename T::element_type element_type;
		auto native_value = static_cast<element_type>(value);
		auto append = (native_value << this->start);
		auto probe = &this->container[this->index];
		while (true) {
			volatile auto old_value = *probe;
			auto new_value = (old_value & ~(this->mask << this->start)) | append;
			if (__sync_bool_compare_and_swap(probe, old_value, new_value)) {
				return;
			}
			std::this_thread::yield();
		}
	}

	inline
	bool safe_increment() const 
	{
		auto probe = &this->container[this->index];
		while (true) {
			volatile auto old_value = *probe;
			auto raw = (old_value >> this->start) & this->mask;
			if (raw == T::max_value()) {
				return true;
			}
			auto increment = 1ULL << this->start;
			auto new_value = old_value + increment;
			if (__sync_bool_compare_and_swap(probe, old_value, new_value)) {
				return false;
			}
			std::this_thread::yield();
		}
	}
};

} // namespace packed::detail

template <typename T, size_t W> 
class vector
{
public:
	typedef T value_type;
	static const size_t value_width = W;  // size of packed element in bits
	typedef uintmax_t element_type;
	static const size_t element_width = sizeof(element_type) * 8;
	static const size_t values_per_element = element_width / value_width;
	typedef std::vector<element_type> container_type;
	typedef vector<T, W> this_type;

	static constexpr bool power_of_2() 
	{ 
		return (value_width & (value_width - 1)) == 0;
	}

	typedef detail::accessor<this_type, container_type, power_of_2()> accessor;
	typedef const detail::accessor<this_type, const container_type, power_of_2()> const_accessor;

public: // Member functions
	explicit vector()
		: m_size(0)
	{}

	explicit vector(size_t count, const T& value = T())
		: m_container(required_elements(count))
		, m_size(count)
	{}

	template <typename InputIt>
	vector(InputIt first, InputIt last)
	{
		assign(first, last);
	}

	vector(const this_type& rhs)
		: m_container(rhs)
		, m_size(rhs.m_size)
	{}

	vector(this_type&& rhs)
		: m_container(std::move(rhs))
		, m_size(rhs.m_size)
	{}

	vector(std::initializer_list<T> init)
		: m_size(0)
	{
		assign(init);
	}

	this_type& operator=(const this_type& rhs)
	{
		m_container = rhs.m_container;
		m_size = rhs.m_size;
		return *this;
	}

	this_type& operator=(this_type&& rhs)
	{
		m_container = std::move(rhs.m_container);
		m_size = rhs.m_size;
		return *this;
	}

	this_type& operator=(std::initializer_list<T> init)
	{
		assign(init);
		return *this;
	}

	void assign(size_t count, const T& value)
	{
		clear();
		for (size_t i = 0; i < count; i++) {
			push_back(value);
		}
	}

	template <typename InputIt>
	void assign(InputIt first, InputIt last)
	{
		clear();
		std::copy(first, last, std::back_inserter(*this));
	}

	void assign(std::initializer_list<T> init)
	{
		clear();
		for (const auto& item : init) {
			push_back(item);
		}
	}

	// TODO: get_allocator()

public: // Element acesss
	accessor at(size_t pos)
	{
		return accessor(m_container, pos);
	}

	const_accessor at(size_t pos) const
	{
		return const_accessor(m_container, pos);
	}

	const_accessor operator[](size_t pos) const
	{
		return const_accessor(m_container, pos);
	}

	accessor operator[](size_t pos) 
	{
		return accessor(m_container, pos);
	}

	accessor front()
	{
		return accessor(m_container, 0);
	}

	constexpr const_accessor front() const
	{
		return const_accessor(m_container, 0);
	}

	accessor back()
	{
		return accessor(m_container, size()-1);
	}

	constexpr const_accessor back() const
	{
		return const_accessor(m_container, size()-1);
	}

public: // Iterators
	// TODO: begin()
	// TODO: cbegin()
	// TODO: end()
	// TODO: cend()
	// TODO: rbegin()
	// TODO: crbegin()
	// TODO: rend()
	// TODO: crend()

public: // Capacity
	bool empty() const 
	{ 
		return m_container.empty(); 
	}

	size_t size() const 
	{
		return m_size; 
	}

	size_t max_size() const
	{
		return m_container.max_size() / value_width;
	}

	void reserve(size_t new_cap)
	{
		m_container.reserve(required_elements(new_cap));
	}
	
	size_t capacity() const 
	{
		return m_container.capacity() * values_per_element; 
	}

	size_t memory_used() const
	{
		return m_container.capacity() * sizeof(element_type);
	}

	static size_t max_value()
	{
		return (1 << value_width) - 1;
	}

public:	// Modifiers
	void clear()
	{
		m_container.clear();
		m_size = 0;
	}

	void reset()
	{
		// both methods seem to take equal amounts of time
		// auto elements = required_elements(size());
		// m_container.clear();
		// m_container.resize(elements);
		std::fill(m_container.begin(), m_container.end(), 0);
	}

	// TODO: insert()
	// TODO: emplace()
	// TODO: erase()

	void push_back(const value_type& value)
	{
		auto storage_size = m_container.size() * values_per_element;
		if (size() == storage_size) {
			m_container.push_back(0);
		}

		(*this)[size()] = value;
		m_size++;
	}

	// TODO: push_back(value_type&& value)
	// TODO: emplace_back()
	// TODO: pop_back()
	// TODO: resize()
	// TODO: swap()

public: // Traits
	static constexpr size_t required_elements(size_t capacity)
	{
		return (value_width * capacity + (element_width-1)) / element_width;
	}

	static constexpr size_t memory_usage(size_t capacity)
	{
		return required_elements(capacity) * sizeof(element_type);
	}

private:
	container_type m_container;
	size_t m_size;

public: // Diagnostics
	void dump()
	{
		printf("container size: %zu (%zu)\n", m_container.size(), size());
		for (const auto& item : m_container) {
			printf("0x%.16lx\n", item);
		}
	}
};

} // namespace packed
