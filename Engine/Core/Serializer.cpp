#include "stdafx.h"
#include "Serializer.h"

bool Serializer::WriteUByte(unsigned char value)
{
	return Write(&value, sizeof value) == sizeof value;
}

bool Serializer::WriteLine(const std::string &value)
{
	bool success = true;
	success &= Write(value.c_str(), value.length()) == value.length();
	success &= WriteUByte(13);
	success &= WriteUByte(10);
	return success;
}