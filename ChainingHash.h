/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

// Standard library includes
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <cmath>
#include <iterator>

// Custom project includes
#include "Hash.h"

// Namespaces to include
using std::vector;
using std::list;
using std::pair;

using std::cout;
using std::endl;

//
// Separate chaining based hash table - derived from Hash
//
template<typename K, typename V>
class ChainingHash : public Hash<K,V> {
private:

public:
    ChainingHash(int n = 11) {
        array.resize(n);
        s = 0;
    }

    ~ChainingHash() {
        this->clear();
    }

    bool empty() {
        return array.empty();
    }

    int size() {
        return s;
    }

    V& at(const K& key) {
        for (auto & listElement : array[hash(key)]){ //iterate through the list at the hash location
            if (listElement.first == key)
                return listElement.second; // only returns value if key is in the list
        }
        throw std::out_of_range("Key not in hash");
    }

    V& operator[](const K& key) {
        for (auto & listElement : array[hash(key)]){ //iterate through the list at the hash location
            if (listElement.first == key)
                return listElement.second; // only returns value if key is in the list
        }
        throw std::out_of_range("Key not in hash");
    }

    int count(const K& key) {
        int num = 0;
        for (auto & listElement : array[hash(key)]){ //iterate through the list at the hash location
            if (listElement.first == key)
                num++; //increment total if keys are the same
        }
        return num; //return total
    }

    void emplace(K key, V value) {
        insert({key,value});
    }

    void insert(const std::pair<K, V>& pair) {
        array[hash(pair.first)].push_front(pair); //push new pair to front of list at hash location
        s++;
        if (load_factor() > 0.75) rehash(); // rehash if load factor is above threshold
    }

    void erase(const K& key) {
        for (auto & listElement : array[hash(key)]){ //iterate through the list at the hash location
            if (listElement.first == key)
                array[hash(key)].remove(listElement); //get rid of the element that matches the given key
        }
    }

    void clear() {
        for (auto &list : array){
            list.clear();
        }
        array.clear();
        s = 0;
    }

    int bucket_count() {
        return array.size();
    }

    int bucket_size(int n) {
        return array[n].size();
    }

    int bucket(const K& key) {
        for (auto & listElement : array[hash(key)]){ //iterate through the list at the hash location
            if (listElement.first == key)
                return hash(key); // only returns bucket number if it is in the list
        }
        throw std::out_of_range("Key not in hash"); // throw exception if not in list
    }

    float load_factor() {
        return s / array.size();
    }

    void rehash() {

    }

    void rehash(int n) {
    }


private:

    vector<list<pair<K,V>>> array;
    int s; //keeps track of the size (number of filled buckets)

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

        return (key * key) % this->bucket_count();       
    }

};

#endif //__CHAINING_HASH_H
