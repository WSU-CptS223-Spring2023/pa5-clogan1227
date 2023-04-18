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
    vector<K,pair<V,EntryState>> array;
    int s; //size of table

public:
    ProbingHash(int n = 11) {
        array.resize(11);
        s = 0;
    }

    ~ProbingHash() {
        this->empty();
    }

    bool empty() {
        return array.empty();
    }

    int size() {
        return s;
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
        array.clear();
        s = 0;
    }

    int bucket_count() {
        return array.capacity();
    }

    int bucket_size(int n) {
        return array[n].second.second == VALID ? 1 : 0;
    }

    int bucket(const K& key) {
        return 1;
    }

    float load_factor() {
        return ((float)s/(float)array.capacity());
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
        std::hash<K> hashFunction;
        return hashFunction(key) % this->bucket_count();       
    }
    
};

#endif //__PROBING_HASH_H
