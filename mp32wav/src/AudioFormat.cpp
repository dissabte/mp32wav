#include "AudioFormat.h"

AudioFormat::AudioFormat()
    : _byteOrder(Endianness::BigEndian)
    , _codec(Codec::Unknown)
    , _sampleType(SampleType::Unknown)
    , _channelsCount(0)
    , _sampleRate(0)
    , _sampleSizeInBits(0)
{
}

bool AudioFormat::isValid() const
{
	return (_codec != Codec::Unknown) &&
	        (_sampleType != SampleType::Unknown) &&
	        (_channelsCount >= 1 && _channelsCount <= 2) &&
	        (_sampleSizeInBits > 0) &&
	        (_sampleRate > 0);
}

AudioFormat::Codec AudioFormat::codec() const
{
	return _codec;
}

void AudioFormat::setCodec(AudioFormat::Codec newCodec)
{
	if (_codec != newCodec)
	{
		_codec = newCodec;
	}
}

Endianness AudioFormat::byteOrder() const
{
	return _byteOrder;
}

void AudioFormat::setByteOrder(Endianness newByteOrder)
{
	if (_byteOrder != newByteOrder)
	{
		_byteOrder = newByteOrder;
	}
}

AudioFormat::SampleType AudioFormat::sampleType() const
{
	return _sampleType;
}

void AudioFormat::setSampleType(AudioFormat::SampleType newSampleType)
{
	if (_sampleType != newSampleType)
	{
		_sampleType = newSampleType;
	}
}

int AudioFormat::channelsCount() const
{
	return _channelsCount;
}

void AudioFormat::setChannelsCount(int newChannelsCount)
{
	if (_channelsCount != newChannelsCount)
	{
		_channelsCount = newChannelsCount;
	}
}

int AudioFormat::sampleRate() const
{
	return _sampleRate;
}

void AudioFormat::setSampleRate(int newSampleRate)
{
	if (_sampleRate != newSampleRate)
	{
		_sampleRate = newSampleRate;
	}
}

int AudioFormat::sampleSizeInBits() const
{
	return _sampleSizeInBits;
}

void AudioFormat::setSampleSizeInBits(int newSampleSizeInBits)
{
	if (_sampleSizeInBits != newSampleSizeInBits)
	{
		_sampleSizeInBits = newSampleSizeInBits;
	}
}

bool AudioFormat::operator ==(const AudioFormat& other) const
{
	return (_byteOrder == other._byteOrder) &&
	        (_sampleType == other._sampleType) &&
	        (_channelsCount == other._channelsCount) &&
	        (_sampleRate == other._sampleRate) &&
	        (_sampleSizeInBits == other._sampleSizeInBits);
}

bool AudioFormat::operator !=(const AudioFormat& other) const
{
	return !(operator ==(other));
}
