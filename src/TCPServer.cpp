#pragma comment(lib, "ws2_32.lib")

#include "TCPServer.h"

#include <iostream>

#include <string>

#include <sstream>

#include <vector>

#include <thread>

#include <chrono>

TCPServer::TCPServer(
    int port,
    LRUCache &cache,
    PersistenceManager &persistenceManager)
    : port(port),
      cache(cache),
      persistenceManager(persistenceManager)
{
    serverSocket = INVALID_SOCKET;

    isRunning = true;
}
std::string TCPServer::executeCommand(const std::string &input)
{
    std::stringstream ss(input);

    std::vector<std::string> tokens;
    std::string word;

    while (ss >> word)
    {
        tokens.push_back(word);
    }

    if (tokens.empty())
    {
        return "Empty Command\n";
    }

    std::string command = tokens[0];

    if (command == "SET")
    {
        if (tokens.size() == 3)
        {
            cache.set(tokens[1], tokens[2]);
            return "OK\n";
        }

        if (tokens.size() == 5 && tokens[3] == "EX")
        {
            int ttlSeconds = std::stoi(tokens[4]);
            cache.set(tokens[1], tokens[2], ttlSeconds);
            return "OK\n";
        }

        return "Usage: SET <key> <value> [EX seconds]\n";
    }

    if (command == "GET")
    {
        if (tokens.size() != 2)
        {
            return "Usage: GET <key>\n";
        }

        return cache.get(tokens[1]) + "\n";
    }

    if (command == "DEL")
    {
        if (tokens.size() != 2)
        {
            return "Usage: DEL <key>\n";
        }

        if (cache.erase(tokens[1]))
        {
            return "Deleted\n";
        }

        return "Key Not Found\n";
    }

    if (command == "SIZE")
    {
        return std::to_string(cache.size()) + "\n";
    }

    if (command == "EXISTS")
    {
        if (tokens.size() != 2)
        {
            return "Usage: EXISTS <key>\n";
        }

        return cache.exists(tokens[1]) ? "true\n" : "false\n";
    }

    if (command == "TTL")
    {
        if (tokens.size() != 2)
        {
            return "Usage: TTL <key>\n";
        }

        return std::to_string(cache.ttl(tokens[1])) + "\n";
    }

    if (command == "CLEAR")
    {
        cache.clear();
        return "Cache Cleared\n";
    }

    if (command == "SAVE")
    {
        persistenceManager.save(cache);
        return "Cache Saved\n";
    }
    if (command == "BENCHMARK")
    {
        const int operations = 100000;

        auto startTime =
            std::chrono::high_resolution_clock::now();

        for (int i = 0; i < operations; i++)
        {
            cache.set(
                "key" + std::to_string(i),
                "value" + std::to_string(i));
        }

        for (int i = 0; i < operations; i++)
        {
            cache.get(
                "key" + std::to_string(i));
        }

        auto endTime =
            std::chrono::high_resolution_clock::now();

        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                endTime - startTime);

        return "Benchmark completed: " +
               std::to_string(operations) +
               " SET and " +
               std::to_string(operations) +
               " GET operations in " +
               std::to_string(duration.count()) +
               " ms\n";
    }
    if (command == "SHUTDOWN")
    {
        persistenceManager.save(cache);

        isRunning = false;

        closesocket(serverSocket);

        return "Server shutting down\n";
    }
    return "Unknown Command\n";
}

void TCPServer::handleClient(SOCKET clientSocket)
{
    std::cout << "Client connected successfully" << std::endl;

    char buffer[1024];

    int bytesReceived =
        recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0);

    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';

        std::cout
            << "Received: "
            << buffer
            << std::endl;

        std::string response =
            executeCommand(buffer);

        send(
            clientSocket,
            response.c_str(),
            response.length(),
            0);
    }

    closesocket(clientSocket);
}

void TCPServer::start()
{
    WSADATA wsaData;

    int result = WSAStartup(
        MAKEWORD(2, 2),
        &wsaData);

    if (result != 0)
    {
        std::cout << "WSAStartup failed" << std::endl;
        return;
    }

    serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "Socket creation failed" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    result = bind(
        serverSocket,
        (sockaddr *)&serverAddress,
        sizeof(serverAddress));

    if (result == SOCKET_ERROR)
    {
        std::cout << "Bind failed. Port may already be in use." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    result = listen(
        serverSocket,
        SOMAXCONN);

    if (result == SOCKET_ERROR)
    {
        std::cout << "Listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "MiniRedis TCP Server listening on port "
              << port
              << std::endl;

    while (isRunning)
    {
        std::cout << "Waiting for client connection..." << std::endl;

        SOCKET clientSocket = accept(
            serverSocket,
            nullptr,
            nullptr);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cout << "Client connection failed" << std::endl;
            continue;
        }

        std::thread clientThread(
            &TCPServer::handleClient,
            this,
            clientSocket);

        clientThread.detach();
    }
}