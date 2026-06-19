#include "Node.h"

Node::Node(const std::string& key,
           const std::string& value)
{
    this->key = key;
    this->value = value;

    hasExpiry = false;
    expiryTime = 0;

    prev = nullptr;
    next = nullptr;
}