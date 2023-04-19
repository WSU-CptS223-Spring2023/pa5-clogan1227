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
    vector<pair<K,EntryState>> array;
    int s; //size of table

public:
    ProbingHash(int n = 101) {
        array.resize(n);
        makeEmpty(); //initialize all spots to empty
    }

    void makeEmpty(){
        for (auto& item: array){
            item.second = EMPTY;
        }
        s = 0;
    }

    ~ProbingHash() {
        this->clear();
    }

    bool empty() {
        return array.empty();
    }

    int size() {
        return s;
    }

    V& at(const K& key) {
        int index = hash(key), i=0;
        while (array[index + i].second != EMPTY){ //while we haven't seen an empty bucket
            if (array[index + i].first == key)
                return array[index + i].second; //if keys match return the corresponding value
            i++;
            if (i == bucket_count()) 
                i = -1 * index; //moves the index to the beginning of the array so it doesn't go beyond the array's bounds
        }
        cout << "Key not in hash" << endl;
    }

    V& operator[](const K& key) {
        int index = hash(key), i=0;
        while (array[index + i].second != EMPTY){ //while we haven't seen an empty bucket
            if (array[index + i].first == key)
                return array[index + i].second; //if keys match return the corresponding value
            i++;
            if (i == bucket_count()) //
                i = -1 * index; //moves the index to the beginning of the array so it doesn't go beyond the array's bounds
        }
        cout << "Key not in hash" << endl;
    }

    int count(const K& key) {
        int index = hash(key), i=0, total=0;
        while (array[index + i].second != EMPTY){ //while we haven't seen an empty bucket
            if (array[index + i].first == key) //if we find a key matching the given value increment the total
                total++;
            i++;
            if (i == bucket_count()) 
                i = -1 * index; //moves the index to the beginning of the array so it doesn't go beyond the array's bounds
        }
        return total;
    }

    void emplace(K key, V value) {
        insert({key,value}); //use insert as helper
    }

    void insert(const std::pair<K, V>& pair) {
        int index = hash(pair.first), i=0;
        while (array[index + i].second != EMPTY){ //while there isn't an empty bucket, increment by 1 (linear probing)
            i++;
            if (i == bucket_count()) 
                i = -1 * index; //moves the index to the beginning of the array so it doesn't go beyond the array's bounds
        }
        array[index + i] = {pair.first, VALID}; //insert the pair in the empty bucket
        s++;
        if (load_factor() > 0.75) //rehash if above load factor
            rehash();
    }

    void erase(const K& key) {
        int index = hash(key), i=0;
        while (array[index + i].first != key){ //while the current index isn't the pair we're looking for, increment by 1 (linear probing)
            if (array[index + i].second == EMPTY){ //if we reach an empty spot, the key doesn't exist, return
                cout << "Key not in hash" << endl;
                return;
            }
            i++;
            if (i == bucket_count())
                i = -1 * index; //moves the index to the beginning of the array so it doesn't go beyond the array's bounds
        }
        array[index + i].second == DELETED; //mark the targeted pair as deleted (lazy deletion)
        s--;
    }

    void clear() {
        array.clear();
        s = 0;
    }

    int bucket_count() {
        return array.capacity();
    }

    int bucket_size(int n) {
        return array[n].second == VALID ? 1 : 0;
    }

    int bucket(const K& key) {
        int index = hash(key), i=0;
        while (array[index + i].second != EMPTY){ //while we haven't seen an empty bucket
            if (array[index + i].first == key)
                return index + i; //if the keys match, return the position
            i++;
            if (i == bucket_count())
                i = -1 * index; //moves the index to the beginning of the array so it doesn't go beyond the array's bounds
        }
        cout << "Key not in hash" << endl;
        return -1; //return invalid position if pair can't be found
    }

    float load_factor() {
        return ((float)s/(float)array.capacity());
    }

    void rehash() {
        vector<pair<K,EntryState>> oldArray = array;

        array.resize(findNextPrime(2 * array.capacity())); //double current size and find prime

        makeEmpty(); //make all states empty //size is reset in function as well

        for (auto& item: oldArray){ //iterate through entire old array
            if (item.second == VALID) //if item is valid
                insert(std::move(item)); //move into newly resized array
        }
    }

    void rehash(int n) {
        vector<pair<K,EntryState>> oldArray = array;

        array.resize(findNextPrime(n)); //find next prime after given value

        makeEmpty(); //make all states empty //size is reset in function as well

        for (auto& item: oldArray){ //iterate through entire old array
            if (item.second == VALID) //if item is valid
                insert(std::move(item)); //move into newly resized array
        }
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
