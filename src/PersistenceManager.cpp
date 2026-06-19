#include "PersistenceManager.h"

#include "LRUCache.h"
#include <sstream>
#include <fstream>

PersistenceManager::PersistenceManager(
    const std::string &fileName)
{
    this->fileName = fileName;
}

void PersistenceManager::save(LRUCache &cache)
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        return;
    }

    Node *current = cache.usageList.getHead();

    while (current != nullptr)
    {
        file
            << current->key << "|"
            << current->value << "|"
            << current->hasExpiry << "|"
            << current->expiryTime
            << "\n";

        current = current->next;
    }

    file.close();
}

void PersistenceManager::load(LRUCache &cache)
{
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string key;
        std::string value;
        std::string hasExpiryText;
        std::string expiryTimeText;

        std::getline(ss, key, '|');
        std::getline(ss, value, '|');
        std::getline(ss, hasExpiryText, '|');
        std::getline(ss, expiryTimeText, '|');

        if (key.empty() || value.empty())
        {
            continue;
        }

        bool hasExpiry = std::stoi(hasExpiryText) == 1;
        long long expiryTime = std::stoll(expiryTimeText);

        cache.set(key, value);

        Node *node = cache.cacheMap[key];
        node->hasExpiry = hasExpiry;
        node->expiryTime = expiryTime;
    }

    file.close();

    cache.removeExpiredKeys();
}