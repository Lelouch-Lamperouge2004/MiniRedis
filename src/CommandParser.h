#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "LRUCache.h"
#include "PersistenceManager.h"
#include <string>

class CommandParser
{
private:
    LRUCache& cache;
    PersistenceManager& persistenceManager;

public:
    CommandParser(
    LRUCache& cache,
    PersistenceManager& persistenceManager);

    void start();
};

#endif