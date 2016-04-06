#pragma once

static const int CONVERSION_BUFFER_LENGTH = 128;

std::string ToString(const char *formatString, ...);
std::string AppendWithFormatArgs(const char *formatString, va_list args);

/// Convert a path to the format required by the operating system in wide characters.
std::wstring GetWideNativePath(const std::string &pathName);