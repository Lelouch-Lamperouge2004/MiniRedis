#include "TCPServer.h"

#include <iostream>

int main()
{
    LRUCache cache(3);

    PersistenceManager persistenceManager(
        "../data/cache.dat");

    persistenceManager.load(cache);

    TCPServer server(
        6379,
        cache,
        persistenceManager);

    server.start();

    return 0;
}