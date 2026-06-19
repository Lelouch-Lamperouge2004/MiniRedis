#ifndef NODE_H
#define NODE_H

#include <string>

class Node
{
public:
    std::string key;
    std::string value;

    bool hasExpiry;
    long long expiryTime;

    Node* prev;
    Node* next;

    Node(const std::string& key,
         const std::string& value);
};

#endif