#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include <string>

class LRUCache;

class PersistenceManager
{
private:
    std::string fileName;

public:
    PersistenceManager(const std::string& fileName);

    void save(LRUCache& cache);

    void load(LRUCache& cache);
};

#endif