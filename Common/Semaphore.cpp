#include "Semaphore.h"

Semaphore::Semaphore(int value) :m_count(value) {}

Semaphore::~Semaphore() {}

void Semaphore::Wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (--m_count < 0)
	{
		m_condition_variable.wait(lock);
	}
}

void Semaphore::Signal()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (++m_count <= 0)
	{
		m_condition_variable.notify_one();
	}
}
