#pragma once


template<typename T>
class Stack
{
    struct Node
    {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* top_ = nullptr;
    int   size_ = 0;

public:
    ~Stack() 
    { 
        while (!empty()) 
            pop(); 
    }
    void push(const T& val) 
    { 
        Node* n = new Node(val); 
        n->next = top_; top_ = n; 
        ++size_;
    }
    void pop() 
    { 
        if (!top_) 
            return;
        Node* t = top_; 
        top_ = top_->next; 
        delete t; 
        --size_; 
    }
    const T& top() const { return top_->data; }
    bool empty() const { return top_ == nullptr; }
    int  size()  const { return size_; }
};