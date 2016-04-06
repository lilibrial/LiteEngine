#pragma once

class Serializer
{
public:
	virtual ~Serializer() = default;

	// // Write bytes to the stream. Return number of bytes actually written.
	virtual unsigned Write(const void *data, unsigned size) = 0;

	// Write an 8-bit unsigned integer.
	bool WriteUByte(unsigned char value);
	
	// Write a text line. Char codes 13 & 10 will be automatically appended.
	bool WriteLine(const std::string &value);
};