#include <cstring>
#include <algorithm>

#include "bigint.h"

namespace bigint {
	static inline int getValidBits(uint32_t num)
	{
		int ret = 0;
		if (num & 0xFFFF0000)
		{
			ret += 16;
			num >>= 16;
		}
		if (num & 0xFFFFFF00)
		{
			ret += 8;
			num >>= 8;
		}
		if (num & 0xFFFFFFF0)
		{
			ret += 4;
			num >>= 4;
		}
		if (num & 0xFFFFFFFC)
		{
			ret += 2;
			num >>= 2;
		}
		if (num & 0xFFFFFFFE)
		{
			ret++;
			num >>= 1;
		}
		if (num)
		{
			ret++;
		}
		return ret;
	}

	template <int SIZE>
	static inline void __shift(uint32_t* value, int shiftnum)
	{
		if (shiftnum == 0)
		{
			return;
		}
		if (shiftnum > 0)
		{
			if (shiftnum >= SIZE * 8 * sizeof(uint32_t))
			{
				std::memset(value, 0, SIZE * sizeof(uint32_t));
			}
			else
			{
				uint32_t tmp[SIZE << 1]{ 0 };
				std::memcpy(tmp + SIZE, value, SIZE * sizeof(uint32_t));
				int shiftInt = shiftnum >> 5;
				int shiftIntBit = shiftnum & 0x01F;
				if (shiftIntBit == 0)
				{
					uint32_t mask = (static_cast<uint32_t>(1) << shiftIntBit) - 1;
					for (int i = SIZE - 1; i >= 0; i--)
					{
						int curIndex = SIZE + i - shiftInt;
						tmp[SIZE + i] = tmp[curIndex];
					}
				}
				else
				{
					uint32_t mask = (static_cast<uint32_t>(1) << shiftIntBit) - 1;
					for (int i = SIZE - 1; i >= 0; i--)
					{
						int curIndex = SIZE + i - shiftInt;
						uint32_t& cur = tmp[curIndex];
						uint32_t& prev = tmp[curIndex - 1];
						tmp[SIZE + i] = (cur >> shiftIntBit) | ((prev & mask) << (32 - shiftIntBit));
					}
				}
				std::memcpy(value, tmp + SIZE, SIZE * sizeof(uint32_t));
			}
		}
		else
		{
			if (shiftnum <= -SIZE * 8 * sizeof(uint32_t))
			{
				std::memset(value, 0, SIZE * sizeof(uint32_t));
			}
			else
			{
				shiftnum = -shiftnum;
				uint32_t tmp[SIZE << 1]{ 0 };
				std::memcpy(tmp, value, SIZE * sizeof(uint32_t));
				int shiftInt = shiftnum >> 5;
				int shiftIntBit = shiftnum & 0x01F;
				if (shiftIntBit == 0)
				{
					uint32_t mask = ~((static_cast<uint32_t>(1) << shiftIntBit) - 1);
					for (int i = 0; i < SIZE; i++)
					{
						int curIndex = i + shiftInt;
						tmp[i] = tmp[curIndex];
					}
				}
				else
				{
					uint32_t mask = ~((static_cast<uint32_t>(1) << shiftIntBit) - 1);
					for (int i = 0; i < SIZE; i++)
					{
						int curIndex = i + shiftInt;
						uint32_t& cur = tmp[curIndex];
						uint32_t& next = tmp[curIndex + 1];
						tmp[i] = (cur << shiftIntBit) | ((next & mask) >> (32 - shiftIntBit));
					}
				}
				std::memcpy(value, tmp, SIZE * sizeof(uint32_t));
			}
		}
	}

	template <int SIZE>
	static inline void __shift2(uint32_t* value, int shiftnum)
	{
		if (shiftnum == 0)
		{
			return;
		}
		if (shiftnum > 0)
		{
			if (shiftnum >= SIZE * 8 * sizeof(uint32_t))
			{
				std::memset(value, 0, SIZE * sizeof(uint32_t));
			}
			else
			{
				int shiftInt = shiftnum >> 5;
				int shiftIntBit = shiftnum & 0x01F;
				if (shiftIntBit == 0)
				{
					uint32_t mask = (static_cast<uint32_t>(1) << shiftIntBit) - 1;
					for (int i = SIZE - 1; i >= shiftInt; i--)
					{
						int curIndex = i - shiftInt;
						value[i] = value[curIndex];
					}
				}
				else
				{
					uint32_t mask = (static_cast<uint32_t>(1) << shiftIntBit) - 1;
					for (int i = SIZE - 1; i > shiftInt; i--)
					{
						int curIndex = i - shiftInt;
						uint32_t& cur = value[curIndex];
						uint32_t& prev = value[curIndex - 1];
						value[i] = (cur >> shiftIntBit) | ((prev & mask) << (32 - shiftIntBit));
					}
					value[shiftInt] >>= shiftIntBit;
				}
				std::memset(value, 0, shiftInt * sizeof(uint32_t));
			}
		}
		else
		{
			if (shiftnum <= -SIZE * 8 * sizeof(uint32_t))
			{
				std::memset(value, 0, SIZE * sizeof(uint32_t));
			}
			else
			{
				shiftnum = -shiftnum;
				int shiftInt = shiftnum >> 5;
				int shiftIntBit = shiftnum & 0x01F;
				if (shiftIntBit == 0)
				{
					uint32_t mask = ~((static_cast<uint32_t>(1) << shiftIntBit) - 1);
					for (int i = 0; i < SIZE - shiftInt; i++)
					{
						int curIndex = i + shiftInt;
						value[i] = value[curIndex];
					}
				}
				else
				{
					uint32_t mask = ~((static_cast<uint32_t>(1) << shiftIntBit) - 1);
					for (int i = 0; i < SIZE - shiftInt - 1; i++)
					{
						int curIndex = i + shiftInt;
						uint32_t& cur = value[curIndex];
						uint32_t& next = value[curIndex + 1];
						value[i] = (cur << shiftIntBit) | ((next & mask) >> (32 - shiftIntBit));
					}
					value[SIZE - shiftInt - 1] <<= shiftIntBit;
				}
				std::memset(value + SIZE - shiftInt, 0, shiftInt * sizeof(uint32_t));
			}
		}
	}

	template <int SIZE>
	static inline void __add(uint32_t* retValue, const uint32_t* left, const uint32_t* right)
	{
		uint32_t overflow = 0;
		for (int i = SIZE - 1; i >= 0; i--)
		{
			uint64_t tmp = static_cast<uint64_t>(left[i]) + right[i] + overflow;
			retValue[i] = tmp;
			overflow = tmp >> 32;
		}
	}
	template <int SIZE>
	static inline void __sub(uint32_t* retValue, const uint32_t* left, const uint32_t* right)
	{
		uint32_t overflow = 1;
		for (int i = SIZE - 1; i >= 0; i--)
		{
			uint64_t tmp = static_cast<uint64_t>(left[i]) + (~right[i]) + overflow;
			retValue[i] = tmp;
			overflow = tmp >> 32;
		}
	}
	template <int SIZE>
	static inline void __mul(uint32_t* retValue, const uint32_t* left, const uint32_t* right)
	{
		uint64_t overflow = 0;
		for (int i = SIZE - 1; i >= 0; i--)
		{
			retValue[i] = overflow;
			overflow >>= 32;
			for (int j = 0; j < SIZE - i; j++)
			{
				uint64_t tmp = static_cast<uint64_t>(left[SIZE - 1 - j]) * right[i + j] + retValue[i];
				retValue[i] = tmp;
				overflow += (tmp >> 32);
			}
		}
	}
	template <int SIZE>
	static inline void __div(uint32_t* retValue, const uint32_t* left, const uint32_t* right)
	{
		uint32_t tmpL[SIZE << 1]{ 0 };
		uint32_t tmpR[SIZE << 1]{ 0 };
		std::memcpy(tmpL + SIZE, left, SIZE * 4);
		std::memcpy(tmpR + SIZE, right, SIZE * 4);
		uint64_t overflow = 0;
		for (int i = SIZE - 1; i >= 0; i--)
		{
			retValue[i] = overflow;
			overflow >>= 32;
			for (int j = 0; j < SIZE - i; j++)
			{
				uint64_t tmp = static_cast<uint64_t>(left[SIZE - 1 - j]) * right[i + j] + retValue[i];
				retValue[i] = tmp;
				overflow += (tmp >> 32);
			}
		}
	}

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

	UBigInt2048 UBigInt2048::operator>>(const int& num) const
	{
		UBigInt2048 retValue(*this);
		__shift2<64>(retValue.b, num);
		return retValue;
	}

	UBigInt2048& UBigInt2048::operator>>=(const int& num)
	{
		__shift2<64>(b, num);
		return *this;
	}

	UBigInt2048 UBigInt2048::operator<<(const int& num) const
	{
		UBigInt2048 retValue(*this);
		__shift2<64>(retValue.b, -num);
		return retValue;
	}

	UBigInt2048& UBigInt2048::operator<<=(const int& num)
	{
		__shift2<64>(b, -num);
		return *this;
	}

	UBigInt2048 UBigInt2048::operator+(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		__add<64>(retValue.b, this->b, other.b);
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator-(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		__sub<64>(retValue.b, this->b, other.b);
		return retValue;
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
		__mul<64>(retValue.b, b, other.b);
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator/(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		__div<64>(retValue.b, b, other.b);
		return retValue;
	}
}

