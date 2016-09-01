#pragma once

#include "MpegDecoder.h"
#include "AudioFile.h"
#include <lame.h>
#include <vector>

class MpegDecoder::Implementation
{
	static const uint32_t kDefaultNumberOfSamples;

	enum MpegVersion
	{
		MPEG1,
		MPEG2,
		MPEG2_5
	};

public:
	Implementation();
	~Implementation();

	bool openFile(const std::string& fileName);

private:
	void initializeLame();
	bool mpegFrameHeaderFound(AudioFile& file);
	bool readId3v2Tag(AudioFile& file, std::vector<char>& id3v2Tag);
	bool skipAlbumIDHeader(AudioFile& file);
	bool anySaneMpegFrameFound(AudioFile& file);
	bool mpegFrameIsSane(const char* buffer);
	bool decodeMp3Frame(AudioFile& file, mp3data_struct& mp3data);

	size_t id3v2TagLength(const char* buffer);

private:
	lame_t _globalFlags;
	hip_t  _hipGlobalFlags;

	AudioFile _inputFile;

	std::vector<char> _id3v2Tag;

	MpegVersion _version;
	int _layer;

	mp3data_struct _mp3data;
	int _encDelay;
	int _encPadding;
	bool _freeFormat;
};
