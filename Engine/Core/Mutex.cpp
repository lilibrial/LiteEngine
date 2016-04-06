#include "stdafx.h"
#include "Mutex.h"

#ifdef _WIN32

Mutex::Mutex() : m_handle(new CRITICAL_SECTION)
{
	InitializeCriticalSection((CRITICAL_SECTION*)m_handle);
}

Mutex::~Mutex()
{
	CRITICAL_SECTION *cs = (CRITICAL_SECTION*)m_handle;
	DeleteCriticalSection(cs);
	delete cs;
	m_handle = nullptr;
}

void Mutex::Acquire()
{
	EnterCriticalSection((CRITICAL_SECTION*)m_handle);
}

bool Mutex::TryAcquire()
{
	return TryEnterCriticalSection((CRITICAL_SECTION*)m_handle) != FALSE;
}

void Mutex::Release()
{
	LeaveCriticalSection((CRITICAL_SECTION*)m_handle);
}

#endif

MutexLock::MutexLock(Mutex &mutex) : m_mutex(mutex)
{
	m_mutex.Acquire();
}

MutexLock::~MutexLock()
{
	m_mutex.Release();
}