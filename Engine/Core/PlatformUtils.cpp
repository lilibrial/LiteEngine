#include "stdafx.h"
#include "PlatformUtils.h"

void PrintUnicode(const std::string &str, bool error)
{
#ifdef _WIN32
	FILE *out = error ? stderr : stdout;
	if ( !_isatty(_fileno(out)) )
		fprintf(out, "%s", str.c_str());
	else
	{
		HANDLE stream = GetStdHandle(error ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
		if ( stream == INVALID_HANDLE_VALUE ) return;
		DWORD charsWritten;
		WriteConsoleA(stream, str.c_str(), str.length(), &charsWritten, 0);
	}
#endif
}

void PrintUnicodeLine(const std::string &str, bool error)
{
	PrintUnicode(str + '\n', error);
}