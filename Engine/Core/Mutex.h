#pragma once

#include "NonCopyable.h"

class Mutex
{
public:
	Mutex();
	~Mutex();

	void Acquire();
	bool TryAcquire();

	void Release();
private:
	void *m_handle = nullptr;
};

class MutexLock : NonCopyable
{
public:
	MutexLock(Mutex &mutex);
	~MutexLock();

private:
	Mutex &m_mutex;
};