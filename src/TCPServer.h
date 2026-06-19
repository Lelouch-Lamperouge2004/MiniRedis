#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include "LRUCache.h"
#include "PersistenceManager.h"
#include <string>
#include <atomic>

class TCPServer
{
private:
    int port;

    SOCKET serverSocket;

    std::atomic<bool> isRunning;

    LRUCache& cache;
    PersistenceManager& persistenceManager;

    std::string executeCommand(const std::string& input);

    void handleClient(SOCKET clientSocket);

public:
    TCPServer(
        int port,
        LRUCache& cache,
        PersistenceManager& persistenceManager);

    void start();
};

#endif