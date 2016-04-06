#pragma once

#include "Serializer.h"
#include "Deserializer.h"

enum FileMode
{
	Read = 0,
	Write,
	ReadWrite
};

class File : public Serializer, public Deserializer
{
public:
	~File() final;

	// Open file. Return true if successful.
	bool Open(const std::string &fileName, FileMode mode = FileMode::Read);
	// Close the file.
	void Close();
	
	// Flush any buffered output to the file.
	void Flush();

	// Write bytes to the file. Return number of bytes actually written.
	unsigned Write(const void *data, unsigned size) final;

	// Return whether is open.
	bool IsOpen() const;

	// Return the file name.
	const std::string& GetName() /*final*/const { return m_fileName; }

private:
	std::string m_fileName;
	FileMode m_mode = FileMode::Read;

	FILE *m_handle = nullptr;

	// Start position within a package file, 0 for regular files.
	unsigned m_offset = 0;
	// Content checksum.
	unsigned m_checksum = 0;
	// Compression flag.
	bool m_compressed = false;
	// Synchronization needed before read -flag.
	bool m_readSyncNeeded = false;
	// Synchronization needed before write -flag.
	bool m_writeSyncNeeded = false;
};