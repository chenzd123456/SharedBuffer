#pragma once

#include <memory>
#include <string>
#include <map>
#include <any>

#include "SharedBufferType.h"
#include "ISharedBuffer.h"
#include "Common/Singleton.h"

class SharedBufferManager
{
	friend class Singleton<SharedBufferManager>;
private:
	std::unique_ptr<std::map<std::string, std::any>> m_map;
	SharedBufferManager();
public:
	static std::shared_ptr<SharedBufferManager> GetInstance();
	static void DesInstance();
	~SharedBufferManager();
	template<typename T>
	std::shared_ptr<ISharedBuffer<T>> CreateSharedBuffer(_in_ std::string buffer_name, _in_ SharedBufferType type, _in_ size_t max_size);
	void DestroySharedBuffer(_in_ std::string buffer_name);
	template<typename T>
	std::shared_ptr<ISharedBuffer<T>> GetSharedBuffer(_in_ std::string buffer_name);
};

inline SharedBufferManager::SharedBufferManager()
{
	m_map = std::make_unique<std::map<std::string, std::any>>();
}

inline std::shared_ptr<SharedBufferManager> SharedBufferManager::GetInstance()
{
	return Singleton<SharedBufferManager>::GetInstance();
}

inline void SharedBufferManager::DesInstance()
{
	Singleton<SharedBufferManager>::DesInstance();
}

inline SharedBufferManager::~SharedBufferManager()
{
}

template<typename T>
inline std::shared_ptr<ISharedBuffer<T>> SharedBufferManager::CreateSharedBuffer(std::string buffer_name, SharedBufferType type, _in_ size_t max_size)
{
	std::shared_ptr<ISharedBuffer<T>> shared_buffer;

	switch (type)
	{
	case SharedBufferType::QUEUE:
		shared_buffer = std::make_shared<SharedQueueBuffer<T>>();
		break;
	default:
		break;
	}

	shared_buffer->SetMaxSize(max_size);
	m_map->insert({ buffer_name , shared_buffer });
	return shared_buffer;
}

inline void SharedBufferManager::DestroySharedBuffer(std::string buffer_name)
{
	m_map->erase(buffer_name);
}

template<typename T>
inline std::shared_ptr<ISharedBuffer<T>> SharedBufferManager::GetSharedBuffer(std::string buffer_name)
{
	try 
	{
		return std::any_cast<std::shared_ptr<ISharedBuffer<T>>>(m_map->at(buffer_name));;
	}
	catch(std::bad_any_cast e)
	{
		return std::shared_ptr<ISharedBuffer<T>>();
	}
}
