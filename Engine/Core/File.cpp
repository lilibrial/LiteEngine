#include "stdafx.h"
#include "File.h"
#include "Log.h"
#include "StringUtils.h"

static const unsigned M_MAX_UNSIGNED = 0xffffffff;

#ifdef _WIN32
static const wchar_t *openMode[] =
{
	L"rb",
	L"wb",
	L"r+b",
	L"w+b"
};
#else
static const char *openMode[] =
{
	"rb",
	"wb",
	"r+b",
	"w+b"
};
#endif

File::~File()
{
	Close();
}

bool File::Open(const std::string &fileName, FileMode mode)
{
	Close();

	if ( fileName.empty() )
	{
		LOGERROR("Could not open file with empty name");
		return false;
	}

#ifdef _WIN32
	// TODO: проверить на ошибки _wfopen_s
	_wfopen_s(&m_handle, GetWideNativePath(fileName).c_str(), openMode[mode]);
#endif

	// If file did not exist in readwrite mode, retry with write-update mode
	if ( mode == FileMode::ReadWrite && !m_handle )
	{
#ifdef _WIN32
		// TODO: проверить на ошибки _wfopen_s
		_wfopen_s(&m_handle, GetWideNativePath(fileName).c_str(), openMode[mode + 1]);
#endif
	}

	if ( !m_handle )
	{
		LOGERRORF("Could not open file %s", fileName.c_str());
		return false;
	}

	m_fileName = fileName;
	m_mode = mode;
	m_position = 0;
	m_offset = 0;
	m_checksum = 0;
	m_compressed = false;
	m_readSyncNeeded = false;
	m_writeSyncNeeded = false;

	fseek(m_handle, 0, SEEK_END);
	long size = ftell(m_handle);
	fseek(m_handle, 0, SEEK_SET);
	if ( size > M_MAX_UNSIGNED )
	{
		LOGERRORF("Could not open file %s which is larger than 4GB", fileName.c_str());
		Close();
		m_size = 0;
		return false;
	}
	m_size = (unsigned)size;
	return true;
}

void File::Close()
{
	if ( m_handle )
	{
		fclose(m_handle);
		m_handle = nullptr;
		m_position = 0;
		m_size = 0;
		m_offset = 0;
		m_checksum = 0;
	}
}

void File::Flush()
{
	if ( m_handle ) fflush(m_handle);
}

unsigned File::Write(const void *data, unsigned size)
{
	if ( !m_handle )
	{
		// Do not log the error further here to prevent spamming the stderr stream
		return 0;
	}

	if ( m_mode == FileMode::Read )
	{
		LOGERROR("File not opened for writing");
		return 0;
	}

	if ( !size )
		return 0;

	// Need to reassign the position due to internal buffering when transitioning from reading to writing
	if ( m_writeSyncNeeded )
	{
		fseek(m_handle, m_position + m_offset, SEEK_SET);
		m_writeSyncNeeded = false;
	}

	if ( fwrite(data, size, 1, m_handle) != 1 )
	{
		// Return to the position where the write began
		fseek(m_handle, m_position + m_offset, SEEK_SET);
		LOGERROR("Error while writing to file " + GetName());
		return 0;
	}

	m_readSyncNeeded = true;
	m_position += size;
	if ( m_position > m_size )
		m_size = m_position;

	return size;
}

bool File::IsOpen() const
{
	return m_handle != nullptr;
}

