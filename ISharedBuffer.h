#pragma once

#include "SharedBufferHeader.h"

template<typename T>
class ISharedBuffer
{
public:
	ISharedBuffer() {};
	virtual ~ISharedBuffer() {};
	virtual bool Push(_in_ T obj) = 0;
	virtual T Pop() = 0;
	virtual void Clear() = 0;
	virtual bool Empty() = 0;
	virtual size_t Size() = 0;
	virtual void SetMaxSize(_in_ size_t size) = 0;
	virtual size_t MaxSize() = 0;
};