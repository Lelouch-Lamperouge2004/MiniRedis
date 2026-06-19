#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <thread>
#include <vector>

void sendCommand(const std::string& command)
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        std::cout << "WSAStartup failed" << std::endl;
        return;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET)
    {
        WSACleanup();
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6379);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    result = connect(
        clientSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress));

    if (result == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    send(
        clientSocket,
        command.c_str(),
        command.length(),
        0);

    char buffer[1024];

    recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0);

    closesocket(clientSocket);
    WSACleanup();
}

int main()
{
    const int clientCount = 200;

    std::vector<std::thread> threads;

    for (int i = 0; i < clientCount; i++)
    {
        std::string command =
            "SET stress_key_" +
            std::to_string(i) +
            " value_" +
            std::to_string(i);

        threads.emplace_back(sendCommand, command);
    }

    for (std::thread& thread : threads)
    {
        thread.join();
    }

    std::cout << "Stress test completed with "
              << clientCount
              << " concurrent clients"
              << std::endl;

    return 0;
}