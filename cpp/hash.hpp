#pragma once

#include <array>
#include <limits>
#include <string>

struct string_hasher {
  explicit string_hasher(const std::string& value) : value(value) {}

  size_t operator()(size_t seed) const {
    size_t hash = 0;
    for (const auto& ch : value) {
      hash = primes[seed] * hash + ch;
    }
    return hash;
  }

  const std::string value;

  const std::array<size_t, 10> primes = {{
      5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
  }};
};

struct prime_hasher {
  explicit prime_hasher(size_t value) : value(value), zero_digest(compute(0)) {}

  size_t operator()(size_t seed) const {
    if (seed == 0) {
      return zero_digest;
    }
    return compute(seed);
  }

  size_t compute(size_t seed) const {
    typedef unsigned __int128 uint128_t;
    uint128_t big = (uint128_t)value * (uint128_t)primes[seed];
    return big % std::numeric_limits<uint64_t>::max();
  }

  const std::array<size_t, 10> primes = {
      {11304120250909662091ULL, 14238857486369442079ULL,
       18020468069336417183ULL, 15850096573827867343ULL,
       10176742935504226271ULL, 16185669894312422891ULL,
       15838073050089372361ULL, 10640936780000504717ULL,
       15674341118187572551ULL, 9702237477049335997ULL}};

  const size_t value;
  const size_t zero_digest;
};

template <class T> struct hash {};

template <> struct hash<char*> {
  size_t operator()(const char* value) const {
    string_hasher fn(value);
    return fn(0);
  }
};

template <> struct hash<const char*> {
  size_t operator()(const char* value) const {
    string_hasher fn(value);
    return fn(0);
  }
};

template <> struct hash<std::string> {
  size_t operator()(const std::string& value) const {
    string_hasher fn(value);
    return fn(0);
  }
};
