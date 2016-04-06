#include "stdafx.h"
#include "Timer.h"

std::string Time::GetTimeStamp()
{
	const size_t arraySize = 26;	// количество символов получаемых из ctime_s
	
	time_t sysTime;
	time(&sysTime);
	char dateTime[arraySize];
	// TODO: проверить на ошибки ctime_s
	ctime_s(dateTime, arraySize, &sysTime);

	std::string s(dateTime);

	std::replace(s.begin(), s.end(), '\n', ' ');
	return s;
}