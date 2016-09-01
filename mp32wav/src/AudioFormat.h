#pragma once

#include "AudioUtililty.h"

class AudioFormat
{
public:
	enum class SampleType
	{
		Unknown,
		SignedInteger,
		UnsignedInteger,
		Float
	};

	enum Codec
	{
		Unknown,
		Raw,
		Wave,
		Aiff,
		MP1,                  /* MPEG Layer 1, aka mpg */
		MP2,                  /* MPEG Layer 2 */
		MP3,                  /* MPEG Layer 3 */
		MP123,                /* MPEG Layer 1,2 or 3; whatever .mp3, .mp2, .mp1 or .mpg contains */
		Ogg
	};

public:
	explicit AudioFormat();
	explicit AudioFormat(const AudioFormat& other) = default;
	explicit AudioFormat(AudioFormat&& other) = default;
	~AudioFormat() = default;

	bool isValid() const;

	AudioFormat::Codec codec() const;
	void setCodec(AudioFormat::Codec newCodec);

	Endianness byteOrder() const;
	void setByteOrder(Endianness newByteOrder);

	AudioFormat::SampleType sampleType() const;
	void setSampleType(AudioFormat::SampleType newSampleType);

	int channelsCount() const;
	void setChannelsCount(int newChannelsCount);

	int sampleRate() const;
	void setSampleRate(int newSampleRate);

	int sampleSizeInBits() const;
	void setSampleSizeInBits(int newSampleSizeInBits);

	bool operator ==(const AudioFormat& other) const;
	bool operator !=(const AudioFormat& other) const;

private:
	Endianness _byteOrder;
	AudioFormat::Codec _codec;
	AudioFormat::SampleType _sampleType;
	int _channelsCount;
	int _sampleRate;
	int _sampleSizeInBits;
};
