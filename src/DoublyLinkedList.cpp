#include "DoublyLinkedList.h"

#include <iostream>

DoublyLinkedList::DoublyLinkedList()
{
    head = nullptr;
    tail = nullptr;
}
void DoublyLinkedList::addToFront(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    node->prev = nullptr;
    node->next = head;

    if (head != nullptr)
    {
        head->prev = node;
    }

    head = node;

    if (tail == nullptr)
    {
        tail = node;
    }
}

void DoublyLinkedList::removeNode(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node == head)
    {
        head = node->next;
    }

    if (node == tail)
    {
        tail = node->prev;
    }

    if (node->prev != nullptr)
    {
        node->prev->next = node->next;
    }

    if (node->next != nullptr)
    {
        node->next->prev = node->prev;
    }

    node->prev = nullptr;
    node->next = nullptr;
}

void DoublyLinkedList::moveToFront(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node == head)
    {
        return;
    }

    removeNode(node);
    addToFront(node);
}

Node* DoublyLinkedList::removeFromBack()
{
    if (tail == nullptr)
    {
        return nullptr;
    }

    Node* nodeToRemove = tail;

    removeNode(nodeToRemove);

    return nodeToRemove;
}

void DoublyLinkedList::display()
{
    Node* current = head;

    while (current != nullptr)
    {
        std::cout << current->key << ":" << current->value << " ";
        current = current->next;
    }

    std::cout << std::endl;
}

Node* DoublyLinkedList::getHead()
{
    return head;
}

Node* DoublyLinkedList::getTail()
{
    return tail;
}

void DoublyLinkedList::reset()
{
    head = nullptr;
    tail = nullptr;
}