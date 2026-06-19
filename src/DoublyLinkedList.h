#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "Node.h"

class DoublyLinkedList
{
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList();

    void addToFront(Node* node);

    void removeNode(Node* node);

    void moveToFront(Node* node);

    Node* removeFromBack();

    void display();

    Node* getHead();

    Node* getTail();

    void reset();
};

#endif