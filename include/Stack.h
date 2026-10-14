#ifndef STACK_H
#define STACK_H

template <typename E>
class Stack{
private:
    Stack(const Stack<E> &other){}
    void operator =(const Stack<E> &other){}

public:
    Stack(){}
    virtual ~Stack(){}
    virtual void push(E element) =0;
    virtual E pop() =0;
    virtual E topValue() =0;
    virtual void clear() =0;
    virtual bool isEmpty() =0;
    virtual int getSize() =0;
    virtual void print() =0;
};

#endif // STACK_H
