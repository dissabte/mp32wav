#include "MpegDecoder.h"
#include "MpegDecoderImpl.h"

MpegDecoder::MpegDecoder()
    : AudioDecoder()
    , _impl(new Implementation())
{
}

MpegDecoder::~MpegDecoder()
{
}

bool MpegDecoder::openFile(const std::string& fileName)
{
	if (_impl)
	{
		return _impl->openFile(fileName);
	}
	return false;
}


//void AudioFileDecoder::decode(const std::string& inFile, const std::string& outFile)
//{
//	lame_t globalFlags = lame_init();
//	if (globalFlags == nullptr)
//	{
//		std::cerr << "Couldn't initialize libmp3lame\n";
//		return;
//	}

//	// change globalFlags if needed

//	int result = lame_init_params(globalFlags);
//	if (result < 0)
//	{
//		std::cerr << "lame_init_params returned: " << result << std::endl;
//		lame_close (globalFlags);
//		return;
//	}

//	// init input file:
//	// - check if file is mpeg
//	// - open it and set parameters like num samples, sample rate, etc.

//	// in case we won't be able to read num samples from file - set default:
//	lame_set_num_samples(globalFlags, std::numeric_limits<uint32_t>::max());

//	lame_decode_initfile()

//}

