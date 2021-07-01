#include <cstring>
#include <algorithm>

#include "bigint.h"

namespace bigint {
	UBigInt2048::UBigInt2048()
	{
		std::memset(b, 0, sizeof(b));
	}

	UBigInt2048::UBigInt2048(uint8_t* buf, int num)
	{
		std::memset(b, 0, sizeof(b));
		if (num > 0)
		{
			num = std::min(num, 256);
			int bufIndex = 0;
			if ((num & 0x03) != 0)
			{
				bufIndex = (num % 4) - 3;
				uint32_t n2 = (bufIndex >= 0) ? static_cast<uint32_t>(buf[bufIndex]) << 16 : 0;
				bufIndex++;
				uint32_t n3 = (bufIndex >= 0) ? static_cast<uint32_t>(buf[bufIndex]) << 8 : 0;
				bufIndex++;
				uint32_t n4 = (bufIndex >= 0) ? static_cast<uint32_t>(buf[bufIndex]) << 0 : 0;
				bufIndex++;
				b[64 - num / 4 - 1] = n2 | n3 | n4;
			}
			for (int i = 64 - num / 4; i < 64; i++)
			{
				uint32_t n1 = static_cast<uint32_t>(buf[bufIndex + 0]) << 24;
				uint32_t n2 = static_cast<uint32_t>(buf[bufIndex + 1]) << 16;
				uint32_t n3 = static_cast<uint32_t>(buf[bufIndex + 2]) << 8;
				uint32_t n4 = static_cast<uint32_t>(buf[bufIndex + 3]) << 0;
				b[i] = n1 | n2 | n3 | n4;
				bufIndex += 4;
			}
		}
	}

	void UBigInt2048::getBytes256(uint8_t* buf) const
	{
		int bufIndex = 0;
		for (int i = 0; i < 64; i++)
		{
			buf[bufIndex] = b[i] >> 24;
			bufIndex++;
			buf[bufIndex] = b[i] >> 16;
			bufIndex++;
			buf[bufIndex] = b[i] >> 8;
			bufIndex++;
			buf[bufIndex] = b[i] >> 0;
			bufIndex++;
		}
	}


	UBigInt2048 UBigInt2048::operator*(const uint32_t& other) const
	{
		UBigInt2048 retValue;
		uint32_t overflow = 0;
		for (int i = 63; i >= 0; i--)
		{
			uint64_t tmp = static_cast<uint64_t>(this->b[i]) * other + overflow;
			retValue.b[i] = tmp;
			overflow = tmp >> 32;
		}
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator*(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		uint64_t overflow = 0;
		for (int i = 63; i >= 0; i--)
		{
			retValue.b[i] = overflow;
			overflow >>= 32;
			for (int j = 0; j < 64 - i; j++)
			{
				uint64_t tmp = static_cast<uint64_t>(this->b[63 - j]) * other.b[i + j] + retValue.b[i];
				retValue.b[i] = tmp;
				overflow += (tmp >> 32);
			}
		}
		return retValue;
	}
}

