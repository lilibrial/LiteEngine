#include "stdafx.h"
#include "StringUtils.h"
#include "Log.h"

std::string AppendWithFormatArgs(const char *formatString, va_list args)
{
	int pos = 0, lastPos = 0;
	int length = (int)strlen(formatString);

	std::string ret = "";

	while ( true )
	{
		while ( pos < length && formatString[pos] != '%' ) pos++;
		ret += std::string(formatString + lastPos, (unsigned)(pos - lastPos));
		if ( pos >= length ) return ret;

		char format = formatString[pos + 1];
		pos += 2;
		lastPos = pos;

		switch ( format )
		{
			// Integer
		case 'd':
		case 'i':
			{
				int arg = va_arg(args, int);
				ret += std::to_string(arg);
				break;
			}

			// Unsigned
		case 'u':
			{
				unsigned arg = va_arg(args, unsigned);
				ret += std::to_string(arg);
				break;
			}

			// Unsigned long
		case 'l':
			{
				unsigned long arg = va_arg(args, unsigned long);
				ret += std::to_string(arg);
				break;
			}

			// Real
		case 'f':
			{
				double arg = va_arg(args, double);
				ret += std::to_string(arg);
				break;
			}

			// Character
		case 'c':
			{
				int arg = va_arg(args, int);
				ret += std::to_string(arg);
				break;
			}

			// C string
		case 's':
			{
				char *arg = va_arg(args, char*);
				ret += std::string(arg);
				break;
			}

			// Hex
		case 'x':
			{
				char buf[CONVERSION_BUFFER_LENGTH];
				int arg = va_arg(args, int);
				int arglen = ::sprintf_s(buf, "%x", arg);
				ret += std::string(buf, (unsigned)arglen);
				break;
			}

			// Pointer
		case 'p':
			{
				char buf[CONVERSION_BUFFER_LENGTH];
				int arg = va_arg(args, int);
				int arglen = ::sprintf_s(buf, "%p", reinterpret_cast<void*>(arg));
				ret += std::string(buf, (unsigned)arglen);
				break;
			}

		case '%':
			ret += std::string("%", 1);
			break;

		default:
			LOGWARNINGF("Unsupported format specifier: '%c'", format);
			break;
		}
	}

	return ret;
}


std::string ToString(const char *formatString, ...)
{
	std::string ret;
	va_list args;
	va_start(args, formatString);
	ret = AppendWithFormatArgs(formatString, args);
	va_end(args);
	return ret;
}

std::wstring GetWideNativePath(const std::string &pathName)
{
#ifdef _WIN32
	std::wstring ws(pathName.begin(), pathName.end());
	std::replace(ws.begin(), ws.end(), L'/', L'\\');
	return ws;
#endif
}