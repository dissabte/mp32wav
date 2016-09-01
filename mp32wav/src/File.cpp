#include "File.h"

File::File(const std::string& filePath)
    : _path(filePath)
{
}

File::~File()
{
}

std::string File::path() const
{
	return _path;
}

void File::setPath(const std::string& newPath)
{
	if (_path != newPath)
	{
		if (isOpen())
		{
			close();
		}
		_path = newPath;
	}
}

bool File::isOpen() const
{
	return _file.is_open();
}

File::Mode File::mode() const
{
	return _mode;
}

bool File::isAtEnd() const
{
	return _file.eof();
}

bool File::open(File::Mode mode)
{
	bool opened = false;
	if (!_path.empty())
	{
		std::ios_base::openmode openMode = std::ios_base::binary;
		if (mode & ReadOnly)
		{
			openMode |= std::ios_base::in;
		}
		if (mode & WriteOnly)
		{
			openMode |= std::ios_base::out;
		}
		_file.open(_path, openMode);
		_mode = mode;
		opened = _file.good();
	}
	return opened;
}

void File::close()
{
	_file.close();
}

File::size_type File::size()
{
	const pos_type currentPosition = _file.tellg();
	_file.seekg(0, std::ios_base::end);
	const pos_type fileLastPos = _file.tellg();
	_file.seekg(currentPosition);
	return static_cast<size_type>(fileLastPos);
}

bool File::seek(File::size_type distance, File::SeekOrigin origin)
{
	std::ios_base::seekdir dir = std::ios_base::beg;
	switch (origin)
	{
	case SeekFromBeginning:
		break; // already set

	case SeekFromCurrent:
		dir = std::ios_base::cur;
		break;

	case SeekFromEnd:
		dir = std::ios_base::end;
		break;
	}
	_file.seekg(distance, dir);
	return _file.good();
}

File::size_type File::read(char* buffer, File::size_type maxReadSize)
{
	_file.read(buffer, maxReadSize);
	return _file.gcount();
}

File::size_type File::write(const char* buffer, File::size_type maxReadSize)
{
	const pos_type currentPos = _file.tellp();
	_file.write(buffer, maxReadSize);
	return _file.tellp() - currentPos;
}
