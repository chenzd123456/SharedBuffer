#include <stdlib.h>

#include "ISharedBuffer.h"
#include "SharedQueueBuffer.h"
#include "SharedBufferManager.h"
#include "windows.h"


class Buffer
{
private:
	std::shared_ptr<void> m_buffer;
public:
	Buffer() {
		m_buffer = std::shared_ptr<void>(malloc(10*1024*1024)) ;
	};
	~Buffer() {};
};

void thread_func1()
{
	auto shared_buffer = SharedBufferManager::GetInstance()->GetSharedBuffer<int>("int_buffer");

	if (!shared_buffer)
	{
		return;
	}

	while (1)
	{
		for (int i = 0; i < 100; i++)
		{
			shared_buffer->Push(i);
			Sleep(10);
		}
	}
}

void thread_func2()
{
	auto shared_buffer = SharedBufferManager::GetInstance()->GetSharedBuffer<int>("int_buffer");

	if (!shared_buffer)
	{
		return;
	}

	while (1)
	{
		for (int i = 0; i < 100; i++)
		{
			auto a = shared_buffer->Pop();
			printf("%d\n", a);
			Sleep(100);
		}
	}
}

int main(int argc, char** argv)
{
	auto manager = SharedBufferManager::GetInstance();
	//auto shared_buffer = manager->CreateSharedBuffer<std::shared_ptr<Buffer>>("int_buffer", SharedBufferType::QUEUE, 1);
	manager->CreateSharedBuffer<int>("int_buffer", SharedBufferType::QUEUE, 1);

	std::thread t1(thread_func1);
	std::thread t2(thread_func2);

	while (1)
	{
		Sleep(5 * 1000);
	}
}