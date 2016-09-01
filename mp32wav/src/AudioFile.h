#pragma once

#include "File.h"
#include "AudioUtililty.h"

class AudioFile : public File
{
public:
	explicit AudioFile() = default;
	explicit AudioFile(const std::string& filePath);
	virtual ~AudioFile() = default;

public:
	template<typename T>
	T readBits()
	{
		T result = T();
		read(reinterpret_cast<char*>(&result.bytes), sizeof(T));
		return result;
	}
};
