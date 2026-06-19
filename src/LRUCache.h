#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "DoublyLinkedList.h"
#include "PersistenceManager.h"
#include <string>
#include <unordered_map>
#include <mutex>

class LRUCache
{
    friend class PersistenceManager;

private:
    int capacity;
    int currentSize;

    bool isExpired(Node *node);
    void removeExpiredKeys();

    std::mutex cacheMutex;

    std::unordered_map<std::string, Node *> cacheMap;
    DoublyLinkedList usageList;

public:
    LRUCache(int capacity);

    std::string get(const std::string &key);

    void set(const std::string &key,
             const std::string &value);

    void set(const std::string &key,
             const std::string &value,
             int ttlSeconds);

    bool erase(const std::string &key);

    void display();

    int size();

    bool exists(const std::string& key);

    long long ttl(const std::string& key);

    void clear();
};

#endif