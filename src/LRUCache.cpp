#include "LRUCache.h"

#include <chrono>
#include <iostream>
#include <mutex>

LRUCache::LRUCache(int capacity)
{
    this->capacity = capacity;
    this->currentSize = 0;
}

bool LRUCache::isExpired(Node *node)
{
    if (node == nullptr)
    {
        return true;
    }

    if (!node->hasExpiry)
    {
        return false;
    }

    long long currentTime =
        std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());

    return currentTime >= node->expiryTime;
}

void LRUCache::removeExpiredKeys()
{
    Node *current = usageList.getHead();

    while (current != nullptr)
    {
        Node *nextNode = current->next;

        if (isExpired(current))
        {
            usageList.removeNode(current);
            cacheMap.erase(current->key);
            delete current;
            currentSize--;
        }

        current = nextNode;
    }
}

std::string LRUCache::get(const std::string &key)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = cacheMap.find(key);

    if (it == cacheMap.end())
    {
        return "NULL";
    }

    Node *node = it->second;

    if (isExpired(node))
    {
        usageList.removeNode(node);
        cacheMap.erase(key);
        delete node;
        currentSize--;

        return "NULL";
    }

    usageList.moveToFront(node);

    return node->value;
}

void LRUCache::set(const std::string &key,
                   const std::string &value)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    if (cacheMap.find(key) != cacheMap.end())
    {
        Node *existingNode = cacheMap[key];

        existingNode->value = value;
        existingNode->hasExpiry = false;
        existingNode->expiryTime = 0;

        usageList.moveToFront(existingNode);

        return;
    }

    if (currentSize == capacity)
    {
        Node *leastRecentlyUsedNode = usageList.removeFromBack();

        if (leastRecentlyUsedNode != nullptr)
        {
            cacheMap.erase(leastRecentlyUsedNode->key);
            delete leastRecentlyUsedNode;
            currentSize--;
        }
    }

    Node *newNode = new Node(key, value);

    usageList.addToFront(newNode);
    cacheMap[key] = newNode;
    currentSize++;
}

void LRUCache::set(const std::string &key,
                   const std::string &value,
                   int ttlSeconds)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    if (cacheMap.find(key) != cacheMap.end())
    {
        Node *existingNode = cacheMap[key];

        existingNode->value = value;
        existingNode->hasExpiry = true;

        long long currentTime =
            std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now());

        existingNode->expiryTime = currentTime + ttlSeconds;

        usageList.moveToFront(existingNode);

        return;
    }

    if (currentSize == capacity)
    {
        Node *leastRecentlyUsedNode = usageList.removeFromBack();

        if (leastRecentlyUsedNode != nullptr)
        {
            cacheMap.erase(leastRecentlyUsedNode->key);
            delete leastRecentlyUsedNode;
            currentSize--;
        }
    }

    Node *newNode = new Node(key, value);

    newNode->hasExpiry = true;

    long long currentTime =
        std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());

    newNode->expiryTime = currentTime + ttlSeconds;

    usageList.addToFront(newNode);
    cacheMap[key] = newNode;
    currentSize++;
}

bool LRUCache::erase(const std::string &key)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = cacheMap.find(key);

    if (it == cacheMap.end())
    {
        return false;
    }

    Node *node = it->second;

    usageList.removeNode(node);
    cacheMap.erase(key);
    delete node;
    currentSize--;

    return true;
}

void LRUCache::display()
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    removeExpiredKeys();

    usageList.display();
}
int LRUCache::size()
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    removeExpiredKeys();

    return currentSize;
}
bool LRUCache::exists(const std::string &key)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = cacheMap.find(key);

    if (it == cacheMap.end())
    {
        return false;
    }

    Node *node = it->second;

    if (isExpired(node))
    {
        usageList.removeNode(node);
        cacheMap.erase(key);
        delete node;
        currentSize--;

        return false;
    }

    return true;
}
long long LRUCache::ttl(const std::string &key)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = cacheMap.find(key);

    if (it == cacheMap.end())
    {
        return -1;
    }

    Node *node = it->second;

    if (isExpired(node))
    {
        usageList.removeNode(node);
        cacheMap.erase(key);
        delete node;
        currentSize--;

        return -1;
    }

    if (!node->hasExpiry)
    {
        return -1;
    }

    long long currentTime =
        std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());

    return node->expiryTime - currentTime;
}
void LRUCache::clear()
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    Node* current = usageList.getHead();

    while (current != nullptr)
    {
        Node* nextNode = current->next;

        delete current;

        current = nextNode;
    }

    cacheMap.clear();

    currentSize = 0;

    usageList.reset();
}