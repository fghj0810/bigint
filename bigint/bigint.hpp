#pragma once

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <compare>

namespace bigint {
	template <size_t BITWIDTH>
	class BigUInt final
	{
	public:
		constexpr static size_t getSize() { return BITWIDTH / 8; }
		constexpr static size_t getElementBitWidth() { return sizeof(uint32_t) * 8; }
		constexpr static size_t getElementCount() { return getSize() / sizeof(uint32_t); }
	public:
		BigUInt() = default;
		~BigUInt() = default;
		BigUInt(const BigUInt&) = default;
		BigUInt(BigUInt&&) = default;
		BigUInt& operator=(const BigUInt& other) = default;
		BigUInt& operator=(BigUInt&& other) = default;
		template <size_t OBITWIDTH> explicit BigUInt(const BigUInt<OBITWIDTH>&);

	public:
		BigUInt(const uint32_t& value);
		BigUInt(uint8_t* buf, size_t num);
		void getBytes(uint8_t* buf) const;
		static bool div(const BigUInt& a, const uint32_t& b, BigUInt& q, BigUInt& r) noexcept;
		static bool div(const BigUInt& a, const BigUInt& b, BigUInt& q, BigUInt& r) noexcept;

	public:
		std::strong_ordering operator<=>(const uint32_t& other) const noexcept;
		std::strong_ordering operator<=>(const BigUInt& other) const noexcept;
		BigUInt operator>>(const uint16_t& shiftnum) const;
		BigUInt& operator>>=(const uint16_t& shiftnum);
		BigUInt operator<<(const uint16_t& shiftnum) const;
		BigUInt& operator<<=(const uint16_t& shiftnum);
		BigUInt operator+(const uint32_t& other) const;
		BigUInt& operator+=(const uint32_t& other);
		BigUInt operator+(const BigUInt& other) const;
		BigUInt& operator+=(const BigUInt& other);
		BigUInt operator-(const uint32_t& other) const;
		BigUInt& operator-=(const uint32_t& other);
		BigUInt operator-(const BigUInt& other) const;
		BigUInt& operator-=(const BigUInt& other);
		BigUInt operator*(const uint32_t& other) const;
		BigUInt& operator*=(const uint32_t& other);
		BigUInt operator*(const BigUInt& other) const;
		BigUInt& operator*=(const BigUInt& other);
		BigUInt operator/(const uint32_t& other) const;
		BigUInt& operator/=(const uint32_t& other);
		BigUInt operator/(const BigUInt& other) const;
		BigUInt& operator/=(const BigUInt& other);
		BigUInt operator%(const uint32_t& other) const;
		BigUInt& operator%=(const uint32_t& other);
		BigUInt operator%(const BigUInt& other) const;
		BigUInt& operator%=(const BigUInt& other);
		BigUInt& operator++();
		BigUInt& operator--();

	private:
		uint32_t b[getElementCount()] = { 0 };
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
	template<size_t BITWIDTH>template<size_t OBITWIDTH>
	inline BigUInt<BITWIDTH>::BigUInt(const BigUInt<OBITWIDTH>& value)
	{
		size_t al = getElementCount();
		size_t vl = BigUInt<OBITWIDTH>::getElementCount();
		size_t l = std::min(al, vl);
		uint32_t* srcP = (uint32_t*)(&value);
		std::memcpy(b + al - l, srcP + vl - l, l * sizeof(uint32_t));
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>::BigUInt(const uint32_t& value)
	{
		b[getElementCount() - 1] = value;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>::BigUInt(uint8_t* buf, size_t num)
	{
		num = std::min(num, getSize());
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
			b[getElementCount() - num / 4 - 1] = n2 | n3 | n4;
		}
		for (size_t i = getElementCount() - num / 4; i < getElementCount(); i++)
		{
			uint32_t n1 = static_cast<uint32_t>(buf[bufIndex + 0]) << 24;
			uint32_t n2 = static_cast<uint32_t>(buf[bufIndex + 1]) << 16;
			uint32_t n3 = static_cast<uint32_t>(buf[bufIndex + 2]) << 8;
			uint32_t n4 = static_cast<uint32_t>(buf[bufIndex + 3]) << 0;
			b[i] = n1 | n2 | n3 | n4;
			bufIndex += 4;
		}
	}
	template<size_t BITWIDTH>
	inline void BigUInt<BITWIDTH>::getBytes(uint8_t* buf) const
	{
		int bufIndex = 0;
		for (int i = 0; i < getElementCount(); i++)
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
	template<size_t BITWIDTH>
	inline bool BigUInt<BITWIDTH>::div(const BigUInt& a, const uint32_t& b, BigUInt& q, BigUInt& r) noexcept
	{
		q = BigUInt<BITWIDTH>();
		r = BigUInt<BITWIDTH>();
		if (b <= 1)
		{
			if (b == 0)
			{
				return false;
			}
			else
			{
				q = a;
			}
		}
		else
		{
			auto cmp = a <=> b;
			if (cmp < 0)
			{
				r = a;
			}
			else if (cmp == 0)
			{
				q.b[getElementCount() - 1] = 0x01;
			}
			else if (cmp > 0)
			{
				uint64_t qq = a.b[0];
				if (qq > b)
				{
					q.b[0] = static_cast<uint32_t>(qq / b);
					qq %= b;
				}
				for (size_t i = 1; i < getElementCount(); i++)
				{
					qq = (qq << 32) | a.b[i];
					q.b[i] = static_cast<uint32_t>(qq / b);
					qq %= b;
				}
				r.b[getElementCount() - 1] |= static_cast<uint32_t>(qq);
			}
		}
		return true;
	}
	template<size_t BITWIDTH>
	inline bool BigUInt<BITWIDTH>::div(const BigUInt& a, const BigUInt& b, BigUInt& q, BigUInt& r) noexcept
	{
		q = BigUInt<BITWIDTH>();
		r = BigUInt<BITWIDTH>();
		if (b <= 1)
		{
			if (b.b[getElementCount() - 1] == 0)
			{
				return false;
			}
			else
			{
				q = a;
			}
		}
		else
		{
			auto cmp = a <=> b;
			if (cmp < 0)
			{
				r = a;
			}
			else if (cmp == 0)
			{
				q.b[getElementCount() - 1] = 0x01;
			}
			else
			{
				const size_t bitsNumPerEle = getElementBitWidth();
				r.b[getElementCount() - 1] |= a.b[0] >> (bitsNumPerEle - 1);
				if (r >= b)
				{
					r -= b;
					q.b[getElementCount() - 1] = 0x01;
				}
				for (size_t i = 1; i < BITWIDTH; i++)
				{
					size_t bIndex = i / bitsNumPerEle;
					size_t bitIndex = i % bitsNumPerEle;
					uint32_t mask = static_cast<uint32_t>(1) << ((bitsNumPerEle - 1) - bitIndex);
					r <<= 1;
					q <<= 1;
					r.b[getElementCount() - 1] |= (a.b[bIndex] & mask) >> ((bitsNumPerEle - 1) - bitIndex);
					if (r >= b)
					{
						r -= b;
						q.b[getElementCount() - 1] |= 0x01;
					}
				}
			}
		}
		return true;
	}
	template<size_t BITWIDTH>
	inline std::strong_ordering BigUInt<BITWIDTH>::operator<=>(const uint32_t& other) const noexcept
	{
		for (int i = 0; i < getElementCount() - 1; i++)
		{
			if (b[i] != 0)
			{
				return std::strong_ordering::greater;
			}
		}
		if (b[getElementCount() - 1] != other)
		{
			if (b[getElementCount() - 1] > other)
			{
				return std::strong_ordering::greater;
			}
			else
			{
				return std::strong_ordering::less;
			}
		}
		return std::strong_ordering::equivalent;
	}
	template<size_t BITWIDTH>
	inline std::strong_ordering BigUInt<BITWIDTH>::operator<=>(const BigUInt<BITWIDTH>& other) const noexcept
	{
		for (size_t i = 0; i < getElementCount(); i++)
		{
			if (b[i] != other.b[i])
			{
				if (b[i] > other.b[i])
				{
					return std::strong_ordering::greater;
				}
				else
				{
					return std::strong_ordering::less;
				}
			}
		}
		return std::strong_ordering::equivalent;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator>>(const uint16_t& shiftnum) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		retValue >>= shiftnum;
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator>>=(const uint16_t& shiftnum)
	{
		if (shiftnum != 0)
		{
			if (shiftnum >= BITWIDTH)
			{
				std::memset(b, 0, getSize());
			}
			else
			{
				size_t shiftInt = shiftnum / getElementBitWidth();
				size_t shiftIntBit = shiftnum % getElementBitWidth();
				if (shiftIntBit == 0)
				{
					for (size_t i = getElementCount() - 1; i >= shiftInt; i--)
					{
						size_t curIndex = i - shiftInt;
						b[i] = b[curIndex];
					}
				}
				else
				{
					for (size_t i = getElementCount() - 1; i > shiftInt; i--)
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
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator<<(const uint16_t& shiftnum) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		retValue <<= shiftnum;
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator<<=(const uint16_t& shiftnum)
	{
		if (shiftnum != 0)
		{
			if (shiftnum >= BITWIDTH)
			{
				std::memset(b, 0, getSize());
			}
			else
			{
				size_t shiftInt = shiftnum / getElementBitWidth();
				size_t shiftIntBit = shiftnum % getElementBitWidth();
				if (shiftIntBit == 0)
				{
					for (size_t i = 0; i < getElementCount() - shiftInt; i++)
					{
						size_t curIndex = i + shiftInt;
						b[i] = b[curIndex];
					}
				}
				else
				{
					for (size_t i = 0; i < getElementCount() - shiftInt - 1; i++)
					{
						size_t curIndex = i + shiftInt;
						uint32_t& cur = b[curIndex];
						uint32_t& next = b[curIndex + 1];
						b[i] = (cur << shiftIntBit) | (next >> (32 - shiftIntBit));
					}
					b[getElementCount() - shiftInt - 1] = b[getElementCount() - 1] << shiftIntBit;
				}
				std::memset(b + getElementCount() - shiftInt, 0, shiftInt * sizeof(uint32_t));
			}
		}
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator+(const uint32_t& other) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		retValue += other;
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator+=(const uint32_t& other)
	{
		if (other != 0)
		{
			uint32_t overflow = other;
			for (size_t i = getElementCount() - 1; i > 0; i--)
			{
				uint64_t tmp = static_cast<uint64_t>(b[i]) + overflow;
				b[i] = static_cast<uint32_t>(tmp);
				overflow = tmp >> 32;
			}
			b[0] += overflow;
		}
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator+(const BigUInt<BITWIDTH>& other) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		retValue += other;
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator+=(const BigUInt<BITWIDTH>& other)
	{
		uint32_t overflow = 0;
		for (size_t i = getElementCount() - 1; i > 0; i--)
		{
			uint64_t tmp = static_cast<uint64_t>(b[i]) + other.b[i] + overflow;
			b[i] = tmp;
			overflow = tmp >> 32;
		}
		b[0] += other.b[0] + overflow;
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator-(const uint32_t& other) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		retValue -= other;
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator-=(const uint32_t& other)
	{
		if (other != 0)
		{
			uint64_t a = static_cast<uint64_t>(b[getElementCount() - 1]) + (static_cast<uint64_t>(~other) + 1);
			b[getElementCount() - 1] = static_cast<uint32_t>(a);
			uint32_t overflow = a >> 32;
			if (getElementCount() > 1)
			{
				for (size_t i = getElementCount() - 2; i > 0; i--)
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
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator-(const BigUInt<BITWIDTH>& other) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		retValue -= other;
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator-=(const BigUInt<BITWIDTH>& other)
	{
		uint32_t overflow = 1;
		for (size_t i = getElementCount() - 1; i > 0; i--)
		{
			uint64_t tmp = static_cast<uint64_t>(b[i]) + (static_cast<uint64_t>(~other.b[i]) + overflow);
			b[i] = static_cast<uint32_t>(tmp);
			overflow = tmp >> 32;
		}
		b[0] += (~other.b[0]) + overflow;
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator*(const uint32_t& other) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		retValue *= other;
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator*=(const uint32_t& other)
	{
		if (other == 0)
		{
			std::memset(b, 0, getSize());
		}
		else if (other > 1)
		{
			uint32_t overflow = 0;
			for (size_t i = getElementCount() - 1; i > 0; i--)
			{
				uint64_t tmp = static_cast<uint64_t>(b[i]) * other + overflow;
				b[i] = tmp;
				overflow = tmp >> 32;
			}
			b[0] = b[0] * other + overflow;
		}
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator*(const BigUInt<BITWIDTH>& other) const
	{
		BigUInt<BITWIDTH> retValue(*this);
		uint64_t overflow = 0;
		for (size_t i = getElementCount() - 1; i > 0; i--)
		{
			retValue.b[i] = static_cast<uint32_t>(overflow);
			overflow >>= 32;
			for (int j = 0; j < getElementCount() - i; j++)
			{
				uint64_t tmp = static_cast<uint64_t>(b[getElementCount() - 1 - j]) * other.b[i + j] + retValue.b[i];
				retValue.b[i] = static_cast<uint32_t>(tmp);
				overflow += (tmp >> 32);
			}
		}
		retValue.b[0] = static_cast<uint32_t>(overflow);
		for (size_t j = 0; j < getElementCount(); j++)
		{
			retValue.b[0] = b[getElementCount() - 1 - j] * other.b[j] + retValue.b[0];
		}
		return retValue;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator*=(const BigUInt<BITWIDTH>& other)
	{
		BigUInt<BITWIDTH> tmp = *this * other;
		std::memcpy(b, tmp.b, getElementCount());
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator/(const uint32_t& other) const
	{
		BigUInt<BITWIDTH> quotient;
		BigUInt<BITWIDTH> remainder;
		div(*this, other, quotient, remainder);
		return quotient;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator/=(const uint32_t& other)
	{
		*this = *this / other;
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator/(const BigUInt<BITWIDTH>& other) const
	{
		BigUInt<BITWIDTH> quotient;
		BigUInt<BITWIDTH> remainder;
		div(*this, other, quotient, remainder);
		return quotient;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator/=(const BigUInt<BITWIDTH>& other)
	{
		*this = *this / other;
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator%(const uint32_t& other) const
	{
		BigUInt<BITWIDTH> quotient;
		BigUInt<BITWIDTH> remainder;
		div(*this, other, quotient, remainder);
		return remainder;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator%=(const uint32_t& other)
	{
		*this = *this % other;
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH> BigUInt<BITWIDTH>::operator%(const BigUInt<BITWIDTH>& other) const
	{
		BigUInt<BITWIDTH> quotient;
		BigUInt<BITWIDTH> remainder;
		div(*this, other, quotient, remainder);
		return remainder;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator%=(const BigUInt<BITWIDTH>& other)
	{
		*this = *this % other;
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator++()
	{
		*this += 1;
		return *this;
	}
	template<size_t BITWIDTH>
	inline BigUInt<BITWIDTH>& BigUInt<BITWIDTH>::operator--()
	{
		*this -= 1;
		return *this;
	}
}
