#pragma once

#include <string>
#include <fstream>

class File
{
public:
	enum Mode
	{
		ReadOnly = 0x01,
		WriteOnly = 0x02,
		ReadWrite = ReadOnly | WriteOnly
	};

	enum SeekOrigin
	{
		SeekFromBeginning,
		SeekFromCurrent,
		SeekFromEnd
	};

	using size_type = std::streamsize;
	using pos_type = std::streamoff;

public:
	explicit File() = default;
	explicit File(const std::string& filePath);
	virtual ~File();

	virtual bool open(File::Mode mode);
	virtual void close();

	std::string path() const;
	void setPath(const std::string& newPath);

	bool isOpen() const;
	File::Mode mode() const;

	bool isAtEnd() const;
	size_type size();

	size_type read(char* buffer, size_type maxReadSize);
	size_type write(const char* buffer, size_type maxReadSize);

	bool seek(size_type distance, SeekOrigin origin = SeekFromBeginning);

private:
	std::string _path;
	std::fstream _file;
	Mode _mode;
};
