#pragma once

typedef unsigned ThreadID;

class Thread
{
public:
	static void SetMainThread();
	static ThreadID GetCurrentThreadID();
	static bool IsMainThread();
protected:
	static ThreadID mainThreadID;
};