#include "stdafx.h"
#include "Thread.h"

ThreadID Thread::mainThreadID;

void Thread::SetMainThread()
{
	mainThreadID = GetCurrentThreadID();
}

ThreadID Thread::GetCurrentThreadID()
{
#ifdef _WIN32
	return GetCurrentThreadId();
#endif
}

bool Thread::IsMainThread()
{
#ifdef LIBRIS_THREADING
	return GetCurrentThreadID() == mainThreadID;
#else
	return true;
#endif
}