#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>
#include <cmath>

#include "Hash.h"

using std::vector;
using std::pair;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K,V> { // derived from Hash
private:
    // Needs a table and a size.
    // Table should be a vector of std::pairs for lazy deletion

public:
    ProbingHash(int n = 11) {
    }

    ~ProbingHash() {
        // Needs to actually free all table contents
    }

    bool empty() {
        return 1;
    }

    int size() {
        return 1;
    }

    V& at(const K& key) {
        throw std::out_of_range("Key not in hash");
        return 1;
    }

    V& operator[](const K& key) {
        return 1;
    }

    int count(const K& key) {
        return 1;
    }

    void emplace(K key, V value) {
    }

    void insert(const std::pair<K, V>& pair) {
    }

    void erase(const K& key) {
    }

    void clear() {
    }

    int bucket_count() {
        return 1;
    }

    int bucket_size(int n) {
        return 1;
    }

    int bucket(const K& key) {
        return 1;
    }

    float load_factor() {
        return 1;
    }

    void rehash() {
    }

    void rehash(int n) {
    }

private:
    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    int isPrime(int n)
    {
        for (int i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    int hash(const K& key) {
        return 0;       
    }
    
};

#endif //__PROBING_HASH_H
