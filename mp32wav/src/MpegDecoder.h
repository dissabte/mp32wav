#pragma once

#include "AudioDecoder.h"
#include <memory>

class MpegDecoder : public AudioDecoder
{
public:
	explicit MpegDecoder();
	virtual ~MpegDecoder();

	bool openFile(const std::string& fileName);

private:
	class Implementation;
	std::unique_ptr<Implementation> _impl;
};
