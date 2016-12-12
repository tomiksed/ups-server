//
// Created by justme on 7.12.16.
//
#ifndef UPSSERVER_SAFEQUEUE
#define UPSSERVER_SAFEQUEUE

#include <queue>
#include <mutex>

template <class T>
class ConcurrentQueue {

public:
    ConcurrentQueue();
    ~ConcurrentQueue();

    bool empty();
    unsigned long size();
    T front();
    T back();
    void push(T);
    T pop();

private:
    std::queue<T> *queue;
    std::mutex qMutex;
};

template <class T>
ConcurrentQueue<T>::ConcurrentQueue() {
    this->queue = new std::queue<T>();
}

template <class T>
ConcurrentQueue<T>::~ConcurrentQueue() {
    delete(this->queue);
}

template <class T>
bool ConcurrentQueue<T>::empty() {
    this->qMutex.lock();
        bool empty = this->queue->empty();
    this->qMutex.unlock();

    return empty;
}

template <class T>
unsigned long ConcurrentQueue<T>::size() {
    this->qMutex.lock();
        unsigned long size = this->queue->size();
    this->qMutex.unlock();

    return size;
}

template <class T>
T ConcurrentQueue<T>::front() {
    this->qMutex.lock();
        T t = this->queue->front();
    this->qMutex.unlock();

    return t;
}

template <class T>
T ConcurrentQueue<T>::back() {
    this->qMutex.lock();
        T t = this->queue->back();
    this->qMutex.unlock();

    return t;
}

template <class T>
void ConcurrentQueue<T>::push(T t) {
    this->qMutex.lock();
        this->queue->push(t);
    this->qMutex.unlock();
}

template <class T>
T ConcurrentQueue<T>::pop() {
    this->qMutex.lock();
        T t = this->queue->front();
        this->queue->pop();
    this->qMutex.unlock();

    return t;
}

#endif /* UPSSERVER_SAFEQUEUE */