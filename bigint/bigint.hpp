﻿#pragma once

#include <cstdint>
#include <cstring>
#include <algorithm>

namespace bigint {
	template <size_t SIZE>
	class BigInt final
	{
	public:
		constexpr static size_t getElementLength() { return SIZE / 8 / sizeof(uint32_t); }
	public:
		BigInt() = default;
		~BigInt() = default;
		BigInt(const BigInt&) = default;
		BigInt(BigInt&&) = default;
		BigInt& operator=(const BigInt& other) = default;
		BigInt& operator=(BigInt&& other) = default;

	public:
		BigInt(const uint32_t& value);
		BigInt(uint8_t* buf, size_t num);
		void getBytes(uint8_t* buf) const;

	public:
		std::weak_ordering operator<=>(const uint32_t& other) const;
		std::weak_ordering operator<=>(const BigInt& other) const;
		BigInt operator>>(const uint16_t& shiftnum) const;
		BigInt& operator>>=(const uint16_t& shiftnum);
		BigInt operator<<(const uint16_t& shiftnum) const;
		BigInt& operator<<=(const uint16_t& shiftnum);
		BigInt operator+(const uint32_t& other) const;
		BigInt& operator+=(const uint32_t& other);
		BigInt operator+(const BigInt& other) const;
		BigInt& operator+=(const BigInt& other);
		BigInt operator-(const uint32_t& other) const;
		BigInt& operator-=(const uint32_t& other);
		BigInt operator-(const BigInt& other) const;
		BigInt& operator-=(const BigInt& other);
		BigInt operator*(const uint32_t& other) const;
		BigInt& operator*=(const uint32_t& other);
		BigInt operator*(const BigInt& other) const;
		BigInt& operator*=(const BigInt& other);
		BigInt operator/(const BigInt& other) const;
		BigInt& operator/=(const BigInt& other);

	private:
		uint32_t b[getElementLength()] = { 0 };
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
	}
	template<size_t SIZE>
	inline BigInt<SIZE>::BigInt(const uint32_t& value)
	{
		b[getElementLength() - 1] = value;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>::BigInt(uint8_t* buf, size_t num)
	{
		num = std::min(num, 8 * getElementLength());
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
		for (size_t i = getElementLength() - num / 4; i < getElementLength(); i++)
		{
			uint32_t n1 = static_cast<uint32_t>(buf[bufIndex + 0]) << 24;
			uint32_t n2 = static_cast<uint32_t>(buf[bufIndex + 1]) << 16;
			uint32_t n3 = static_cast<uint32_t>(buf[bufIndex + 2]) << 8;
			uint32_t n4 = static_cast<uint32_t>(buf[bufIndex + 3]) << 0;
			b[i] = n1 | n2 | n3 | n4;
			bufIndex += 4;
		}
	}
	template<size_t SIZE>
	inline void BigInt<SIZE>::getBytes(uint8_t* buf) const
	{
		int bufIndex = 0;
		for (int i = 0; i < getElementLength(); i++)
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
	template<size_t SIZE>
	inline std::weak_ordering BigInt<SIZE>::operator<=>(const uint32_t& other) const
	{
		for (int i = 0; i < getElementLength() - 1; i++)
		{
			if (b[i] != 0)
			{
				return std::weak_ordering::greater;
			}
		}
		if (b[getElementLength() - 1] != other)
		{
			if (b[getElementLength() - 1] > other)
			{
				return std::weak_ordering::greater;
			}
			else
			{
				return std::weak_ordering::less;
			}
		}
		return std::weak_ordering::equivalent;
	}
	template<size_t SIZE>
	inline std::weak_ordering BigInt<SIZE>::operator<=>(const BigInt<SIZE>& other) const
	{
		for (size_t i = 0; i < getElementLength(); i++)
		{
			if (b[i] != other.b[i])
			{
				if (b[i] > other.b[i])
				{
					return std::weak_ordering::greater;
				}
				else
				{
					return std::weak_ordering::less;
				}
			}
		}
		return std::weak_ordering::equivalent;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator>>(const uint16_t& shiftnum) const
	{
		BigInt<SIZE> retValue(*this);
		retValue >>= shiftnum;
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator>>=(const uint16_t& shiftnum)
	{
		if (shiftnum != 0)
		{
			if (shiftnum >= SIZE)
			{
				std::memset(b, 0, getElementLength() * sizeof(uint32_t));
			}
			else
			{
				size_t shiftInt = shiftnum / (8 * sizeof(uint32_t));
				size_t shiftIntBit = shiftnum % (8 * sizeof(uint32_t));
				if (shiftIntBit == 0)
				{
					for (size_t i = getElementLength() - 1; i >= shiftInt; i--)
					{
						size_t curIndex = i - shiftInt;
						b[i] = b[curIndex];
					}
				}
				else
				{
					for (size_t i = getElementLength() - 1; i > shiftInt; i--)
					{
						size_t curIndex = i - shiftInt;
						uint32_t& cur = b[curIndex];
						uint32_t& prev = b[curIndex - 1];
						b[i] = (cur >> shiftIntBit) | (prev << (32 - shiftIntBit));
					}
					b[shiftInt] = b[0] >> shiftIntBit;
				}
				std::memset(b, 0, shiftInt * sizeof(uint32_t));
			}
		}
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator<<(const uint16_t& shiftnum) const
	{
		BigInt<SIZE> retValue(*this);
		retValue <<= shiftnum;
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator<<=(const uint16_t& shiftnum)
	{
		if (shiftnum != 0)
		{
			if (shiftnum >= SIZE)
			{
				std::memset(b, 0, getElementLength() * sizeof(uint32_t));
			}
			else
			{
				size_t shiftInt = shiftnum / (8 * sizeof(uint32_t));
				size_t shiftIntBit = shiftnum % (8 * sizeof(uint32_t));
				if (shiftIntBit == 0)
				{
					for (size_t i = 0; i < getElementLength() - shiftInt; i++)
					{
						size_t curIndex = i + shiftInt;
						b[i] = b[curIndex];
					}
				}
				else
				{
					for (size_t i = 0; i < getElementLength() - shiftInt - 1; i++)
					{
						size_t curIndex = i + shiftInt;
						uint32_t& cur = b[curIndex];
						uint32_t& next = b[curIndex + 1];
						b[i] = (cur << shiftIntBit) | (next >> (32 - shiftIntBit));
					}
					b[getElementLength() - shiftInt - 1] = b[getElementLength() - 1] << shiftIntBit;
				}
				std::memset(b + getElementLength() - shiftInt, 0, shiftInt * sizeof(uint32_t));
			}
		}
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator+(const uint32_t& other) const
	{
		BigInt<SIZE> retValue(*this);
		retValue += other;
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator+=(const uint32_t& other)
	{
		if (other != 0)
		{
			uint32_t overflow = other;
			for (size_t i = getElementLength() - 1; i > 0; i--)
			{
				uint64_t tmp = static_cast<uint64_t>(b[i]) + overflow;
				b[i] = static_cast<uint32_t>(tmp);
				overflow = tmp >> 32;
			}
			b[0] += overflow;
		}
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator+(const BigInt<SIZE>& other) const
	{
		BigInt<SIZE> retValue(*this);
		retValue += other;
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator+=(const BigInt<SIZE>& other)
	{
		uint32_t overflow = 0;
		for (size_t i = getElementLength() - 1; i > 0; i--)
		{
			uint64_t tmp = static_cast<uint64_t>(b[i]) + other.b[i] + overflow;
			b[i] = tmp;
			overflow = tmp >> 32;
		}
		b[0] += other.b[0] + overflow;
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator-(const uint32_t& other) const
	{
		BigInt<SIZE> retValue(*this);
		retValue -= other;
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator-=(const uint32_t& other)
	{
		if (other != 0)
		{
			uint64_t a = static_cast<uint64_t>(b[getElementLength() - 1]) + (static_cast<uint64_t>(~other) + 1);
			b[getElementLength() - 1] = static_cast<uint32_t>(a);
			uint32_t overflow = a >> 32;
			if (getElementLength() > 1)
			{
				for (size_t i = getElementLength() - 2; i > 0; i--)
				{
					uint64_t tmp = static_cast<uint64_t>(b[i]) + 0xFFFFFFFF + overflow;
					b[i] = static_cast<uint32_t>(tmp);
					overflow = tmp >> 32;
				}
				b[0] += 0xFFFFFFFF + overflow;
			}
		}
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator-(const BigInt<SIZE>& other) const
	{
		BigInt<SIZE> retValue(*this);
		retValue -= other;
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator-=(const BigInt<SIZE>& other)
	{
		uint32_t overflow = 1;
		for (size_t i = getElementLength() - 1; i > 0; i--)
		{
			uint64_t tmp = static_cast<uint64_t>(b[i]) + (static_cast<uint64_t>(~other.b[i]) + overflow);
			b[i] = static_cast<uint32_t>(tmp);
			overflow = tmp >> 32;
		}
		b[0] += (~other.b[0]) + overflow;
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator*(const uint32_t& other) const
	{
		BigInt<SIZE> retValue(*this);
		retValue *= other;
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator*=(const uint32_t& other)
	{
		if (other == 0)
		{
			std::memset(b, 0, getElementLength() * sizeof(uint32_t));
		}
		else if (other > 1)
		{
			uint32_t overflow = 0;
			for (size_t i = getElementLength() - 1; i > 0; i--)
			{
				uint64_t tmp = static_cast<uint64_t>(b[i]) * other + overflow;
				b[i] = tmp;
				overflow = tmp >> 32;
			}
			b[0] = b[0] * other + overflow;
		}
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator*(const BigInt<SIZE>& other) const
	{
		BigInt<SIZE> retValue(*this);
		uint64_t overflow = 0;
		for (size_t i = getElementLength() - 1; i > 0; i--)
		{
			retValue.b[i] = static_cast<uint32_t>(overflow);
			overflow >>= 32;
			for (int j = 0; j < getElementLength() - i; j++)
			{
				uint64_t tmp = static_cast<uint64_t>(b[getElementLength() - 1 - j]) * other.b[i + j] + retValue.b[i];
				retValue.b[i] = static_cast<uint32_t>(tmp);
				overflow += (tmp >> 32);
			}
		}
		retValue.b[0] = static_cast<uint32_t>(overflow);
		for (size_t j = 0; j < getElementLength(); j++)
		{
			retValue.b[0] = b[getElementLength() - 1 - j] * other.b[j] + retValue.b[0];
		}
		return retValue;
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator*=(const BigInt<SIZE>& other)
	{
		BigInt<SIZE> tmp = *this * other;
		std::memcpy(b, tmp.b, getElementLength());
		return *this;
	}
	template<size_t SIZE>
	inline BigInt<SIZE> BigInt<SIZE>::operator/(const BigInt<SIZE>& other) const
	{
		if (other > *this)
		{
			return BigInt<SIZE>();
		}
		else if (other == *this)
		{
			return BigInt<SIZE>(1);
		}
		else
		{
			BigInt<SIZE> quotient(*this);
			BigInt<SIZE> remainder(*this);
			int validBitsNum = SIZE;
			for (int i = 0; i < getElementLength(); i++)
			{
				if (other.b[i] == 0)
				{
					validBitsNum -= sizeof(uint32_t) * 8;
				}
				else
				{
					validBitsNum -= (sizeof(uint32_t) * 8 - utility::getValidBits(other.b[i]));
					break;
				}
			}
			quotient >>= validBitsNum - 1;
			if (quotient > 4)
			{
				quotient -= 2;
				remainder -= (other * quotient);
			}
			else
			{
				quotient.b[getElementLength() - 1] = 0;
			}
			while (remainder >= other)
			{
				quotient += 1;
				remainder -= other;
			}
			return quotient;
		}
	}
	template<size_t SIZE>
	inline BigInt<SIZE>& BigInt<SIZE>::operator/=(const BigInt<SIZE>& other)
	{
		BigInt<SIZE> tmp = *this / other;
		std::memcpy(b, tmp.b, getElementLength());
		return *this;
	}
}