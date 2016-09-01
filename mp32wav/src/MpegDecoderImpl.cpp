#include "MpegDecoderImpl.h"
#include "AudioUtililty.h"
#include <limits>
#include <algorithm>
#include <cstring>
#include <iostream>

const uint32_t MpegDecoder::Implementation::kDefaultNumberOfSamples = std::numeric_limits<uint32_t>::max();

MpegDecoder::Implementation::Implementation()
    : _globalFlags(nullptr)
    , _hipGlobalFlags(nullptr)
    , _mp3data{}
{
	initializeLame();
}

MpegDecoder::Implementation::~Implementation()
{

}

void MpegDecoder::Implementation::initializeLame()
{
	if (_globalFlags != nullptr)
	{
		std::cerr << "Lame is already initialized\n";
		return;
	}

	_globalFlags = lame_init();
	if (_globalFlags == nullptr)
	{
		std::cerr << "Couldn't initialize libmp3lame\n";
		return;
	}

	// TODO: setup output
	//hip_set_msgf(global.hip, global_ui_config.silent < 10 ? &frontend_msgf : 0);
	//hip_set_errorf(global.hip, global_ui_config.silent < 10 ? &frontend_errorf : 0);
	//hip_set_debugf(global.hip, &frontend_debugf);
}

bool MpegDecoder::Implementation::openFile(const std::string& fileName)
{
	if (_globalFlags == nullptr)
	{
		std::cerr << "openMpegFile: lame is not initialized\n";
		return false;
	}


	lame_set_num_samples(_globalFlags, kDefaultNumberOfSamples);

	// TODO: take care of unicode filenames etc., pass filename here
	_inputFile.setPath(fileName);
	if (!_inputFile.open(AudioFile::ReadOnly))
		return false;

	while (mpegFrameHeaderFound(_inputFile))
	{
		if (decodeMp3Frame(_inputFile, _mp3data))
		{
			if (-1 == lame_set_num_channels(_globalFlags, _mp3data.stereo))
			{
				std::cerr << "Unsupported number of channels: " << _mp3data.stereo << std::endl;
				return false;
			}
			lame_set_in_samplerate(_globalFlags, _mp3data.samplerate);
			lame_set_num_samples(_globalFlags, _mp3data.nsamp);
			if (lame_get_num_samples(_globalFlags) == kDefaultNumberOfSamples)
			{
				const double fileSize = static_cast<double>(_inputFile.size());
				if (fileSize >= 0)
				{
					// try file size, assume 2 bytes per sample
					if (_mp3data.bitrate > 0)
					{
						const double totalseconds = (fileSize * 8.0 / (1000.0 * _mp3data.bitrate));
						const unsigned long tmp_num_samples = static_cast<unsigned long>(totalseconds * lame_get_in_samplerate(_globalFlags));
						lame_set_num_samples(_globalFlags, tmp_num_samples);
						_mp3data.nsamp = tmp_num_samples;
					}
				}
			}

			// libmp3lame note:
			// turn off automatic writing of ID3 tag data into mp3 stream
			// we have to call it before 'lame_init_params', because that
			// function would spit out ID3v2 tag data.
			lame_set_write_id3tag_automatic(_globalFlags, 0);

			// libmp3lame note:
			// Now that all the options are set, lame needs to analyze them and
			// set some more internal options and check for problems
			int returnValue = lame_init_params(_globalFlags);
			if (returnValue < 0)
			{
				std::cerr << "Fatal error during Lame initialization (" << returnValue << ")\n";
				return false;
			}
			return true;
		}
	}
	return false;
}

bool MpegDecoder::Implementation::mpegFrameHeaderFound(AudioFile& file)
{
	if (!readId3v2Tag(file, _id3v2Tag))
		return false;

	if (!skipAlbumIDHeader(file))
		return false;

	if (!anySaneMpegFrameFound(file))
		return false;

	return true;
}

bool MpegDecoder::Implementation::readId3v2Tag(AudioFile& file, std::vector<char>& id3v2Tag)
{
	const size_t kHeaderSize = 10;
	char header[kHeaderSize] = {};

	file.seek(0);
	id3v2Tag.clear();

	if (kHeaderSize != file.read(header, kHeaderSize))
		return false;

	bool tagAlreadyFound = false;
	while (header[0] == 'I' && header[1] == 'D' && header[2] == '3')
	{
		const size_t tagSize = id3v2TagLength(&header[6]);
		if (tagAlreadyFound)
		{
			file.seek(tagSize, File::SeekFromCurrent);
		}
		else
		{
			id3v2Tag.insert(std::begin(id3v2Tag), header, header + kHeaderSize);
			id3v2Tag.resize(kHeaderSize + tagSize);

			if (tagSize != static_cast<size_t>(file.read(&id3v2Tag[kHeaderSize], tagSize)))
				return false;

			tagAlreadyFound = true;
		}

		if (kHeaderSize != file.read(header, kHeaderSize))
			return false;
	}

	file.seek(-kHeaderSize, File::SeekFromCurrent);
	return true;
}

bool MpegDecoder::Implementation::skipAlbumIDHeader(AudioFile& file)
{
	const size_t kHeaderSize = 6;
	char header[kHeaderSize] = {};

	if (kHeaderSize != file.read(header, kHeaderSize))
		return false;

	if (0 == std::memcmp(header, "AiD\1", 4))
	{
		const size_t headerDataSize = static_cast<unsigned char>(header[4]) + 256 * static_cast<unsigned char>(header[5]);
		if (!file.seek(headerDataSize, File::SeekFromCurrent))
			return false;
	}
	else
	{
		file.seek(-kHeaderSize, File::SeekFromCurrent);
	}
	return true;
}

bool MpegDecoder::Implementation::anySaneMpegFrameFound(AudioFile& file)
{
	const size_t kMpegFrameHeaderSize = 4;
	const size_t kBufferSize = kMpegFrameHeaderSize * 2;
	const size_t kSizeToRead = kBufferSize - kMpegFrameHeaderSize; // consequent reads size
	const size_t kOffsetLimit = kSizeToRead - 1; // to know when to read next headers

	char mpegFrameHeaders[kBufferSize];

	if (kBufferSize != file.read(mpegFrameHeaders, kBufferSize))
		return false;

	size_t frameStartOffset = 0;
	while(!mpegFrameIsSane(mpegFrameHeaders + frameStartOffset))
	{
		if (frameStartOffset == kOffsetLimit)
		{
			frameStartOffset = 0;
			std::memcpy(mpegFrameHeaders, mpegFrameHeaders + kSizeToRead, kMpegFrameHeaderSize);
			if (kSizeToRead != file.read(mpegFrameHeaders + kMpegFrameHeaderSize, kSizeToRead))
				return false;
		}
		else
		{
			frameStartOffset++;
		}
	}
	file.seek(-kMpegFrameHeaderSize, File::SeekFromCurrent);
	return true;
}

bool MpegDecoder::Implementation::mpegFrameIsSane(const char* buffer)
{
	const unsigned char* frame = reinterpret_cast<const unsigned char*>(buffer);

	// frame sync, 11 bits, all must be 1
	if ((frame[0] & 0b11111111) != 0b11111111)
		return false;

	if ((frame[1] & 0b11100000) != 0b11100000)
		return false;

	// version id, 2 bits, valid values are 00, 10, 11.
	unsigned char versionId = (frame[1] & 0b00011000) >> 3;
	switch(versionId)
	{
	case 0b00:
		_version = MPEG2_5;
		break;

	case 0b10:
		_version = MPEG2;
		break;

	case 0b11:
		_version = MPEG1;
		break;

	case 0b01:
	default:
		return false;
	}

	// layer id, 2 bits, valid values are 01, 10, 11
	const unsigned char layerId = (frame[1] & 0b00000110) >> 1;
	switch (layerId)
	{
	default:
	case 0b00:
		return false;

	case 0b01:
		_layer = 3;
		break;

	case 0b10:
		_layer = 2;
		break;

	case 0b11:
		_layer = 1;
		break;
	}

	// bitrate index, 4 bits, value 1111 is not used, 0000 means free format
	const unsigned char bitrateIndex = (frame[2] & 0b11110000) >> 4;
	if (bitrateIndex == 0b1111)
		return false;

	_freeFormat = bitrateIndex == 0x0000;

	// check allowed mode/bitrate for MPEG v1 Layer 2
	if (versionId == 0b11 && layerId == 0b10)
	{
		// channel mode, 2 bits, 00 - stereo, 01 - joint stereo, 10 - dual channel, 11 - single channel
		const unsigned char channelMode = (frame[3] & 0b11000000) >> 6;

		enum ChannelModeGroup
		{
			Mono,
			Stereo,
			All
		};
		const ChannelModeGroup channelModeGroup = (channelMode == 0b11) ? Mono : Stereo;
		// TODO: is free format counted? (bitrate index == 0)
		const ChannelModeGroup allowedModesForLayer2[16] = {All, Mono, Mono, Mono, All, Mono, All, All, All, All, All, Stereo, Stereo, Stereo, Stereo, Stereo};
		const bool channelModeAllowed = allowedModesForLayer2[bitrateIndex] == channelModeGroup || allowedModesForLayer2[bitrateIndex] == All;
		if (!channelModeAllowed)
			return false;
	}

	// sampling frequency index, 2 bits, value 11 is reserved
	unsigned char samplingFrequency = (frame[2] & 0b00001100) >> 2;
	if (samplingFrequency == 0b11)
		return false;

	// emphasis mode, 2 bits, value 10 is reserved
	unsigned char emphasisMode = frame[3] & 0b11;
	if (emphasisMode == 0b10)
		return false;

	return true;
}

bool MpegDecoder::Implementation::decodeMp3Frame(AudioFile& file, mp3data_struct& mp3data)
{
	if (_hipGlobalFlags != nullptr)
	{
		hip_decode_exit(_hipGlobalFlags);
	}
	_hipGlobalFlags = hip_decode_init();

	// libmp3lame note:
	// now parse the current buffer looking for MP3 headers.
	// (as of 11/00: mpglib modified so that for the first frame where
	// headers are parsed, no data will be decoded.
	// However, for freeformat, we need to decode an entire frame,
	// so mp3data->bitrate will be 0 until we have decoded the first
	// frame.  Cannot decode first frame here because we are not
	// yet prepared to handle the output.

	const size_t kMpegHeaderSize = 4;
	const size_t kBufferSize = 100;
	unsigned char buffer[kBufferSize] = {};

	if (kMpegHeaderSize != file.read(reinterpret_cast<char*>(buffer), kMpegHeaderSize))
		return false;

	short int pcm_l[1152] = {};
	short int pcm_r[1152] = {};

	int enc_delay = 0;
	int enc_padding = 0;

	int returnValue = hip_decode1_headersB(_hipGlobalFlags, buffer, kMpegHeaderSize, pcm_l, pcm_r, &mp3data, &enc_delay, &enc_padding);
	if (-1 == returnValue)
		return false;

	// repeat until we decode a valid mp3 header.
	while (!mp3data.header_parsed)
	{
		if (kBufferSize != file.read(reinterpret_cast<char*>(buffer), kBufferSize))
			return false;

		returnValue = hip_decode1_headersB(_hipGlobalFlags, buffer, kBufferSize, pcm_l, pcm_r, &mp3data, &enc_delay, &enc_padding);
		if (-1 == returnValue)
			return false;
	}

	if (mp3data.bitrate == 0 && !_freeFormat)
	{
		return false;
	}

	if (mp3data.totalframes > 0)
	{
		// mpglib found a Xing VBR header and computed nsamp & totalframes
	}
	else
	{
		mp3data.nsamp = kDefaultNumberOfSamples;
	}
	return true;
}

size_t MpegDecoder::Implementation::id3v2TagLength(const char* buffer)
{
	size_t b0 = static_cast<unsigned char>(buffer[0]) & 0b01111111;
	size_t b1 = static_cast<unsigned char>(buffer[1]) & 0b01111111;
	size_t b2 = static_cast<unsigned char>(buffer[2]) & 0b01111111;
	size_t b3 = static_cast<unsigned char>(buffer[3]) & 0b01111111;
	return (((((b0 << 7) + b1) << 7) + b2) << 7) + b3;
}
