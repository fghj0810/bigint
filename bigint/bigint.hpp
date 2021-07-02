#pragma once

#include <cstdint>
#include <cstring>
#include <algorithm>

namespace bigint {
	class UBigInt2048 final
	{
	public:
		UBigInt2048() = default;
		~UBigInt2048() = default;
		UBigInt2048(const UBigInt2048&) = default;
		UBigInt2048(UBigInt2048&&) = default;
		UBigInt2048& operator=(const UBigInt2048& other) = default;
		UBigInt2048& operator=(UBigInt2048&& other) = default;

	public:
		UBigInt2048(uint8_t* buf, int num);
		void getBytes256(uint8_t* buf) const;

	public:
		UBigInt2048 operator>>(const int& num) const;
		UBigInt2048& operator>>=(const int& num);
		UBigInt2048 operator<<(const int& num) const;
		UBigInt2048& operator<<=(const int& num);
		UBigInt2048 operator+(const uint32_t& other) const;
		UBigInt2048 operator+(const UBigInt2048& other) const;
		UBigInt2048 operator-(const uint32_t& other) const;
		UBigInt2048 operator-(const UBigInt2048& other) const;
		UBigInt2048 operator*(const uint32_t& other) const;
		UBigInt2048 operator*(const UBigInt2048& other) const;
		UBigInt2048 operator/(const UBigInt2048& other) const;

	private:
		uint32_t b[64] = { 0 };
	};

	namespace utility {
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
					int shiftInt = shiftnum >> 5;
					int shiftIntBit = shiftnum & 0x01F;
					if (shiftIntBit == 0)
					{
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
		static inline int __cmp(const uint32_t* left, const uint32_t& right)
		{
			for (int i = 0; i < SIZE - 1; i++)
			{
				if (left[i] != 0)
				{
					return 1;
				}
			}
			if (left[SIZE - 1] != right)
			{
				if (left[SIZE - 1] > right)
				{
					return 1;
				}
				else
				{
					return -1;
				}
			}
			return 0;
		}
		template <int SIZE>
		static inline int __cmp(const uint32_t* left, const uint32_t* right)
		{
			for (int i = 0; i < SIZE; i++)
			{
				if (left[i] != right[i])
				{
					if (left[i] > right[i])
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
			return 0;
		}
		template <int SIZE>
		static inline void __add(uint32_t* retValue, const uint32_t* left, const uint32_t& right)
		{
			if (right == 0)
			{
				std::memcpy(retValue, left, SIZE * sizeof(uint32_t));
			}
			else
			{
				uint32_t overflow = right;
				for (int i = SIZE - 1; i >= 0; i--)
				{
					uint64_t tmp = static_cast<uint64_t>(left[i]) + overflow;
					retValue[i] = tmp;
					overflow = tmp >> 32;
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
		static inline void __sub(uint32_t* retValue, const uint32_t* left, const uint32_t& right)
		{
			if (right == 0)
			{
				std::memcpy(retValue, left, SIZE * sizeof(uint32_t));
			}
			else
			{
				uint64_t a = static_cast<uint64_t>(left[SIZE - 1]) + (~right) + 1;
				retValue[SIZE - 1] = a;
				uint32_t overflow = a >> 32;
				for (int i = SIZE - 2; i >= 0; i--)
				{
					uint64_t tmp = static_cast<uint64_t>(left[i]) + 0xFFFFFFFF + overflow;
					retValue[i] = tmp;
					overflow = tmp >> 32;
				}
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
		static inline void __mul(uint32_t* retValue, const uint32_t* left, const uint32_t& right)
		{
			if (right == 0)
			{
				std::memset(retValue, 0, SIZE * sizeof(uint32_t));
			}
			else if (right == 1)
			{
				std::memcpy(retValue, left, SIZE * sizeof(uint32_t));
			}
			else
			{
				uint32_t overflow = 0;
				for (int i = 63; i >= 0; i--)
				{
					uint64_t tmp = static_cast<uint64_t>(left[i]) * right + overflow;
					retValue[i] = tmp;
					overflow = tmp >> 32;
				}
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
			int cmp = __cmp<SIZE>(left, right);
			if (cmp < 0)
			{
				std::memset(retValue, 0, SIZE * sizeof(uint32_t));
				return;
			}
			else if (cmp == 0)
			{
				std::memset(retValue, 0, (SIZE - 1) * sizeof(uint32_t));
				retValue[SIZE - 1] = 0x01;
				return;
			}
			else
			{
				std::memcpy(retValue, left, SIZE * sizeof(uint32_t));
				uint32_t tmp1[SIZE];
				uint32_t tmp2[SIZE];
				int validBitsNum = SIZE * sizeof(uint32_t) * 8;
				for (int i = 0; i < SIZE; i++)
				{
					if (right[i] == 0)
					{
						validBitsNum -= sizeof(uint32_t) * 8;
					}
					else
					{
						validBitsNum -= (sizeof(uint32_t) * 8 - getValidBits(right[i]));
						break;
					}
				}
				__shift<SIZE>(retValue, validBitsNum - 1);
				if (__cmp<SIZE>(retValue, 4) > 0)
				{
					__sub<SIZE>(retValue, retValue, 2);
					__mul<SIZE>(tmp1, right, retValue);
					__sub<SIZE>(tmp2, left, tmp1);
				}
				else
				{
					retValue[SIZE - 1] = 0;
				}
				while (__cmp<SIZE>(tmp2, right) >= 0)
				{
					__add<SIZE>(retValue, retValue, 1);
					__sub<SIZE>(tmp2, tmp2, right);
				}
			}
		}
	}

	UBigInt2048::UBigInt2048(uint8_t* buf, int num)
	{
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
		utility::__shift<64>(retValue.b, num);
		return retValue;
	}

	UBigInt2048& UBigInt2048::operator>>=(const int& num)
	{
		utility::__shift<64>(b, num);
		return *this;
	}

	UBigInt2048 UBigInt2048::operator<<(const int& num) const
	{
		UBigInt2048 retValue(*this);
		utility::__shift<64>(retValue.b, -num);
		return retValue;
	}

	UBigInt2048& UBigInt2048::operator<<=(const int& num)
	{
		utility::__shift<64>(b, -num);
		return *this;
	}

	inline UBigInt2048 UBigInt2048::operator+(const uint32_t& other) const
	{
		UBigInt2048 retValue;
		utility::__add<64>(retValue.b, this->b, other);
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator+(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		utility::__add<64>(retValue.b, this->b, other.b);
		return retValue;
	}

	inline UBigInt2048 UBigInt2048::operator-(const uint32_t& other) const
	{
		UBigInt2048 retValue;
		utility::__sub<64>(retValue.b, this->b, other);
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator-(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		utility::__sub<64>(retValue.b, this->b, other.b);
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator*(const uint32_t& other) const
	{
		UBigInt2048 retValue;
		utility::__mul<64>(retValue.b, b, other);
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator*(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		utility::__mul<64>(retValue.b, b, other.b);
		return retValue;
	}

	UBigInt2048 UBigInt2048::operator/(const UBigInt2048& other) const
	{
		UBigInt2048 retValue;
		utility::__div<64>(retValue.b, b, other.b);
		return retValue;
	}
}
