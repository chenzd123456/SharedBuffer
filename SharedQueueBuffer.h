#pragma once

#include <queue>
#include <memory>
#include <mutex>

#include "SharedBufferHeader.h"
#include "ISharedBuffer.h"
#include "Common/Semaphore.h"

template<typename T>
class SharedQueueBuffer : public ISharedBuffer<T>
{
private:
	size_t m_max_size;
	std::mutex m_queue_mutex;
	Semaphore m_semaphore;
	std::unique_ptr<std::queue<T>> m_queue;
public:
	SharedQueueBuffer();
	virtual ~SharedQueueBuffer();
	virtual bool Push(_in_ T obj) override;
	virtual T Pop() override;
	virtual void Clear() override;
	virtual bool Empty() override;
	virtual size_t Size() override;
	virtual void SetMaxSize(_in_ size_t size) override;
	virtual size_t MaxSize() override;
};

template<typename T>
inline SharedQueueBuffer<T>::SharedQueueBuffer()
{
	m_max_size = 0;
	m_queue = std::make_unique<std::queue<T>>();
}

template<typename T>
inline SharedQueueBuffer<T>::~SharedQueueBuffer()
{
}

template<typename T>
inline bool SharedQueueBuffer<T>::Push(_in_ T obj)
{
	
	auto size = m_queue->size();

	if (m_max_size == 0 || size < m_max_size )
	{
		std::lock_guard<std::mutex> lock(m_queue_mutex);
		m_queue->push(obj);
		m_semaphore.Signal();
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
inline T SharedQueueBuffer<T>::Pop()
{
	
	while (1)
	{
		if (m_queue->empty())
		{
			m_semaphore.Wait();
		}
		else
		{
			std::lock_guard<std::mutex> lock(m_queue_mutex);
			auto obj = m_queue->front();
			m_queue->pop();
			return obj;
		}
	}
}

template<typename T>
inline void SharedQueueBuffer<T>::Clear()
{
	std::lock_guard<std::mutex> lock(m_queue_mutex);
	auto empty_queue = std::make_unique<std::queue<T>>();
	m_queue.swap(empty_queue);
}

template<typename T>
inline bool SharedQueueBuffer<T>::Empty()
{
	return m_queue->empty();
}

template<typename T>
inline size_t SharedQueueBuffer<T>::Size()
{
	return m_queue->size();
}

template<typename T>
inline void SharedQueueBuffer<T>::SetMaxSize(_in_ size_t size)
{
	m_max_size = size;
}

template<typename T>
inline size_t SharedQueueBuffer<T>::MaxSize()
{
	return m_max_size;
}
