#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <utility>
#include <stdexcept>
#include <iostream>
#include "List.h"
#include "DNode.h"

using namespace std;

template <typename E>
class DLinkedList : public List<E>{
private:
    DNode<E> *head;
    DNode<E> *tail;
    DNode<E> *current;
    int size;

public:

    DLinkedList(){
        current = head = new DNode<E>(nullptr, nullptr);
        head->next = tail = new DNode<E>(nullptr, head);
        size=0;
    }
    ~DLinkedList(){
        clear();
        delete head;
        delete tail;
    }

    void insert(E element){
        current->next = current->next->previous
            = new DNode<E>(element, current->next, current);
        size++;
    }

    void append(E element){
        tail->previous = tail->previous->next
            = new DNode<E>(element, tail, tail->previous);
        size++;
    }

    E remove(){
        if(size==0){
            throw runtime_error("List is empty");
        }
        if(current == tail->previous){
            throw runtime_error("No current element");
        }
        E result = current->next->element;
        current->next = current->next->next;
        delete current->next->previous;
        current->next->previous = current;
        size--;
        return result;
    }

    void clear(){
        while(head->next != tail){
            head->next = head->next->next;
            delete head->next->previous;
        }
        tail->previous = head;
        size=0;
    }

    E getElement(){
        if(size==0){
            throw runtime_error("List is empty");
        }
        if(current == tail->previous){
            throw runtime_error("No current element");
        }
        return current->next->element;
    }

    void goToStart(){
        current = head;
    }

    void goToEnd(){
        current = tail->previous;
    }

    void goToPos(int pos){
        if(pos<0 || pos>size){
            throw runtime_error("Index out of bounds");
        }

        current = head;
        for(int i=0; i<pos; i++){
            current = current->next;
        }
    }

    void next(){
        if(current->next != tail){
            current = current->next;
        }
    }

    void previous(){
        if(current!=head){
            current = current->previous;
        }
    }

    bool atStart(){
        return current == head;
    }

    bool atEnd(){
        return current->next == tail;
    }

    int getPos(){
        int pos=0;
        DNode<E> *temp = head;
        while(temp !=current){
            temp = temp->next;
            pos++;
        }
        return pos;
    }

    int getSize(){
        return size;
    }

    bool isEmpty(){
        return size==0;
    }

    bool contains(E element) {
        DNode<E> *temp = head;
        while (temp != tail->previous) {
            temp = temp->next;
            if (element == temp->element) {
                return true;
            }
        }
        return false;
    }
};


#endif // DLINKEDLIST_H
