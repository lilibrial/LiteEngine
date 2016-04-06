#include "stdafx.h"
#include "Log.h"
#include "Thread.h"
#include "Timer.h"
#include "PlatformUtils.h"

const char *logLevelPrefixes[] =
{
	"",
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR",
	""
};

static Log *logInstance = nullptr;
static bool threadErrorDisplayed = false;

Log::Log() :
#ifdef _DEBUG
	m_level(LogLevel::Debug)
#else
	m_level(LogLevel::Info)
#endif
{
	logInstance = this;
}

Log::~Log()
{
	logInstance = nullptr;
}

void Log::Open(const std::string &fileName)
{
	if ( fileName.empty() ) return;

	if ( m_logFile && m_logFile->IsOpen() )
	{
		if ( m_logFile->GetName() == fileName ) return;
		else Close();
	}

	m_logFile = std::make_unique<File>();
	if ( m_logFile->Open(fileName, FileMode::Write) )
		Write(LogLevel::Info, "Opened log file " + fileName);
	else
	{
		m_logFile.reset();
		Write(LogLevel::Error, "Failed to create log file " + fileName);
	}
}

void Log::Close()
{
	if ( m_logFile && m_logFile->IsOpen() )
	{
		m_logFile->Close();
		m_logFile.reset();
	}
}

void Log::Write(LogLevel level, const std::string &message)
{
	if ( !logInstance || logInstance->m_level > level ) return;

	if ( level == LogLevel::Raw )
	{
		WriteRaw(message, false);
		return;
	}
	
	// если сейчас не основной поток, то накапливаем записи в список
	if ( !Thread::IsMainThread() )
	{
		MutexLock lock(logInstance->m_logMutex);
		logInstance->m_threadMessages.push_back(StoredLogMessage(message, level, false));
		return;
	}
	
	logInstance->m_lastMessage = message;

	// Собираем строку сообщения
	std::string formattedMsg = logLevelPrefixes[static_cast<size_t>(level)];
	formattedMsg += ": " + message;
	if ( logInstance->m_timeStamp ) formattedMsg = "[ " + Time::GetTimeStamp() + "] " + formattedMsg;

	// выводим строку в поток для сообщений
	PrintUnicodeLine(formattedMsg, level == LogLevel::Error);

	// пишем строку в файл
	if ( logInstance->m_logFile )
	{
		logInstance->m_logFile->WriteLine(formattedMsg);
		logInstance->m_logFile->Flush();
	}
}

void Log::WriteRaw(const std::string &message, bool error)
{
	if ( !logInstance ) return;

	// если сейчас не основной поток, то накапливаем записи в список
	if ( !Thread::IsMainThread() )
	{
		MutexLock lock(logInstance->m_logMutex);
		logInstance->m_threadMessages.push_back(StoredLogMessage(message, LogLevel::Raw, error));
		return;
	}
	
	logInstance->m_lastMessage = message;

	// выводим строку в поток для сообщений
	PrintUnicode(message, error);
	
	// пишем строку в файл
	if ( logInstance->m_logFile )
	{
		logInstance->m_logFile->Write(message.c_str(), message.length());
		logInstance->m_logFile->Flush();
	}
}

void Log::handleEndFrame()
{
	if ( !Thread::IsMainThread() )
	{
		if ( !threadErrorDisplayed )
		{
			fprintf(stderr, "Thread::mainThreadID is not setup correctly! Threaded log handling disabled\n");
			threadErrorDisplayed = true;
		}
		return;
	}

	if ( m_threadMessages.empty() ) return;

	MutexLock lock(m_logMutex);

	// Process messages accumulated from other threads (if any)
	while ( !m_threadMessages.empty() )
	{
		const StoredLogMessage &stored = m_threadMessages.front();

		if ( stored.level != LogLevel::Raw )
			Write(stored.level, stored.message);
		else
			WriteRaw(stored.message, stored.error);

		m_threadMessages.pop_front();
	}
}
