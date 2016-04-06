#pragma once

#include "Mutex.h"
#include "File.h"
#include "StringUtils.h"

enum class LogLevel
{
	Raw = 0,
	Debug,
	Info,
	Warning,
	Error,
	None
};

struct StoredLogMessage
{
	StoredLogMessage() = default;
	StoredLogMessage(const std::string &msg, LogLevel lev, bool err) : message(msg), level(lev), error(err) {}

	std::string message;
	LogLevel level;
	bool error;
};

class Log
{
	friend class Engine;
public:
	Log();
	~Log();

	// Open the log file.
	void Open(const std::string &fileName);
	// Close the log file.
	void Close();

	static void Write(LogLevel level, const std::string &message);
	static void WriteRaw(const std::string &message, bool error = false);

private:
	void handleEndFrame();

	Mutex m_logMutex;
	std::list<StoredLogMessage> m_threadMessages;
	std::unique_ptr<File> m_logFile;

	std::string m_lastMessage;

	LogLevel m_level;

	bool m_timeStamp = true;
};

#ifdef LIBRIS_LOGGING
#define LOGDEBUG(message) Log::Write(LogLevel::Debug, message)
#define LOGINFO(message) Log::Write(LogLevel::Info, message)
#define LOGWARNING(message) Log::Write(LogLevel::Warning, message)
#define LOGERROR(message) Log::Write(LogLevel::Error, message)
#define LOGRAW(message) Log::WriteRaw(message)
#define LOGDEBUGF(format, ...) Log::Write(LogLevel::Debug, ToString(format, ##__VA_ARGS__))
#define LOGINFOF(format, ...) Log::Write(LogLevel::Info, ToString(format, ##__VA_ARGS__))
#define LOGWARNINGF(format, ...) Log::Write(LogLevel::Warning, ToString(format, ##__VA_ARGS__))
#define LOGERRORF(format, ...) Log::Write(LogLevel::Error, ToString(format, ##__VA_ARGS__))
#define LOGRAWF(format, ...) Log::WriteRaw(ToString(format, ##__VA_ARGS__))
#else
#define LOGDEBUG(message) ((void)0)
#define LOGINFO(message) ((void)0)
#define LOGWARNING(message) ((void)0)
#define LOGERROR(message) ((void)0)
#define LOGRAW(message) ((void)0)
#define LOGDEBUGF(...) ((void)0)
#define LOGINFOF(...) ((void)0)
#define LOGWARNINGF(...) ((void)0)
#define LOGERRORF(...) ((void)0)
#define LOGRAWF(...) ((void)0)
#endif