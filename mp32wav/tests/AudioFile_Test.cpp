#include <UnitTest++/UnitTest++.h>
#include <fstream>
#include "../src/AudioFile.h"

SUITE(AudilFileTest)
{
	class AudioFileFixture
	{
	public:
		const int32_t testValue = 35;

		AudioFileFixture()
		{
			std::fstream file;
			file.open("test.data", std::ios_base::out);
			file.write(reinterpret_cast<const char*>(&testValue), sizeof(testValue));
		}
	};

	TEST_FIXTURE(AudioFileFixture, ReadEndiannessTest)
	{
		AudioFile file("test.data");
		CHECK(file.open(AudioFile::ReadOnly));
		int32_t littleEndian = file.readBits<LittleEndian<int32_t>>();
		CHECK_EQUAL(littleEndian, testValue);

		file.seek(0);
		int32_t bigEndian = file.readBits<BigEndian<int32_t>>();
		CHECK(bigEndian != testValue);
	}
}
