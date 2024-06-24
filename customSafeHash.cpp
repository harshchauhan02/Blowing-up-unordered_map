#include <iostream>
#include <unordered_map>
#include <chrono>
#include <ctime>

// Standard hash function example
struct standard_hash {
    size_t operator()(uint64_t x) const {
        return x; // Simple identity hash function, vulnerable to collisions
    }
};

// Improved hash function to add non-determinism
struct custom_hash {
    size_t operator()(uint64_t x) const {
        // Adding non-determinism using high-precision clock
        static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
        return x + FIXED_RANDOM; // Mixing input with a fixed random value to make hacking difficult
    }
};

// Further improvement: using a robust hash function
struct robust_hash {
    static uint64_t splitmix64(uint64_t x) {
        // SplitMix64 hash function implementation for high-quality hashing
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31); // Ensures high-quality random distribution of bits
    }

    size_t operator()(uint64_t x) const {
        // Adding non-determinism and applying SplitMix64 hash function
        static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM); // Combining input with a fixed random value and SplitMix64
    }
};

// Function to test the performance of the hash map
void insert_numbers(long long x) {
    clock_t begin = clock();
    std::unordered_map<long long, int, robust_hash> numbers; // Using robust_hash for protection

    // Insert numbers into the map
    for (int i = 1; i <= 200000; i++)
        numbers[i * x] = i;

    long long sum = 0;
    // Calculate sum to use map and prevent optimization
    for (auto &entry : numbers)
        sum += (entry.first / x) * entry.second;

    printf("x = %lld: %.3lf seconds, sum = %lld\n", x, (double) (clock() - begin) / CLOCKS_PER_SEC, sum);
}

int main() {
    // Testing with different primes
    insert_numbers(107897);
    insert_numbers(126271);
    return 0;
}


/* 
#OUTPUT : 
standard_hash - x = 107897: 11.292 seconds, sum = 2666686666700000
standard_hash - x = 126271: 0.028 seconds, sum = 2666686666700000
custom_hash - x = 107897: 9.524 seconds, sum = 2666686666700000
custom_hash - x = 126271: 0.031 seconds, sum = 2666686666700000
robust_hash - x = 107897: 0.062 seconds, sum = 2666686666700000
robust_hash - x = 126271: 0.046 seconds, sum = 2666686666700000

=====
Used: 14389 ms, 5780 KB


CONCLUSION: 
the performance tests show significant differences in execution time based on the hash function used:

1. standard_hash*:
   - x = 107897: 11.292 seconds
   - x = 126271: 0.028 seconds
Hence, the identity hash function is highly susceptible to collisions, causing severe performance degradation with certain inputs.

2. custom_hash**:
   - x = 107897: 9.524 seconds
   - x = 126271: 0.031 seconds
therefore, adding non-determinism reduces the risk of collision attacks but does not eliminate it entirely, as performance can still be poor with specific adversarial inputs.

3. robust_hash:
   - x = 107897: 0.062 seconds
   - x = 126271: 0.046 seconds
   The SplitMix64-based hash function provides consistent and fast performance, effectively mitigating collision attacks and ensuring a much  better distribution of hash values.

Hence, The robust_hash is a safe hash map. It demonstrates superior performance and resilience against collision attacks, making it the much recommended choice for high-performance, secure hash maps.

*/

