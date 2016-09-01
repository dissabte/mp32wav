#include <UnitTest++/UnitTest++.h>
#include "../src/LittleBigEndian.h"
#include <cstring>

SUITE(AudilFileTest)
{


	TEST(LittleEndianTest)
	{
		const uint32_t testValue = 0x00AACCBB;
		LittleEndian<uint32_t> leValue;
		std::memcpy(leValue.bytes, &testValue, sizeof(uint32_t));
		uint32_t leCheckValue = leValue;
		CHECK_EQUAL(leCheckValue, testValue);

		unsigned char buffer[4] = {0xAA, 0xBB, 0xCC, 0xDD};
		leCheckValue = LittleEndian<uint32_t>::fromBuffer(buffer);
		CHECK_EQUAL(leCheckValue, 0xDDCCBBAA);
	}

	TEST(BigEndianTest)
	{
		const uint32_t testValue = 0x00AACCBB;
		BigEndian<uint32_t> beValue;
		std::memcpy(beValue.bytes, &testValue, sizeof(uint32_t));
		uint32_t beCheckValue = beValue;
		CHECK_EQUAL(beCheckValue, 0xBBCCAA00);

		unsigned char buffer[4] = {0xAA, 0xBB, 0xCC, 0xDD};
		beCheckValue = BigEndian<uint32_t>::fromBuffer(buffer);
		CHECK_EQUAL(beCheckValue, 0xAABBCCDD);
	}
}
