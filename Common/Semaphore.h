#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore
{
private:
    int m_count;
    std::mutex m_mutex;
    std::condition_variable m_condition_variable;
public:
    Semaphore(int value = 0);
    ~Semaphore();
    void Wait();
    void Signal();
};
