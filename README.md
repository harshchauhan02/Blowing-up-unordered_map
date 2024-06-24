# Blowing-up-unordered_map
C++ has long provided data structures like `std::set` and `std::map`, which offer O(log n) operations. With C++11, we gained `std::unordered_set` and `std::unordered_map`, which provide average O(1) operations. However, competitive programming platforms like Codeforces have shown that these hash maps can be exploited, causing performance issues. This article explains how these vulnerabilities occur and offers strategies to safeguard your hash maps.

#The Problem with Unordered Maps

We assume hash maps perform O(1) operations due to their uniform key distribution, resulting in O(1) collisions on average. However, this assumption fails with non-random inputs, especially if adversaries craft inputs to cause collisions, leading to O(n^2) performance degradation.

# Demonstrating the Vulnerability

We can demonstrate how an `unordered_map` can degrade to O(n^2) by understanding its implementation in GCC:

- `unordered_map` uses `__detail::_Mod_range_hashing` and `__detail::_Prime_rehash_policy`.
- It hashes the input and mods by a prime number for bucket positioning.
- The hash function for integers is the identity function (`std::hash`).

By inserting multiples of certain primes, we can force many collisions. The prime depends on the compiler version:
- GCC 6 or earlier: Use 126271.
- GCC 7 or later: Use 107897.

check demonstration.cpp
