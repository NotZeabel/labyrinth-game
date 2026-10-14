#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include <stdexcept>
#include <iostream>
#include "Stack.h"
#include "Node.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class LinkedStack : public Stack<E>{

private:
    Node<E> *top;
    int size;

public:

    LinkedStack(){
        top = nullptr;
        size = 0;
    }
    ~LinkedStack(){
        clear();
    }

    void push(E element){
        top = new Node<E>(element, top);
        size++;
    }

    E pop(){
        if(size==0){
            throw runtime_error("Stack underflow");
        }
        E element = top->element;
        Node<E> *temp = top;
        top = top->next;
        delete temp;
        size--;
        return element;
    }

    E topValue(){
        if(size == 0){
            throw runtime_error("Stack is empty");
        }
        return top->element;
    }

    void clear(){
        while(top!=nullptr){
            Node<E> *temp = top;
            top = top->next;
            delete temp;
        }
        size=0;
    }

    bool isEmpty(){
        return size==0;
    }

    int getSize(){
        return size;
    }

    void print(){
        Node<E> *temp = top;

        cout << "[";
        while(temp!=nullptr){
            cout << temp->element << " ";
            temp = temp->next;
        }
        cout << "]";
        cout << endl;
    }

};



#endif // LINKEDSTACK_H
