#pragma once

#include <cstdint>

namespace bigint {
	class UBigInt2048
	{
	public:
		UBigInt2048();
		UBigInt2048(uint8_t* buf, int num);

		void getBytes256(uint8_t* buf) const;

	public:
		UBigInt2048 operator+(const UBigInt2048& other) const;
		UBigInt2048 operator-(const UBigInt2048& other) const;
		UBigInt2048 operator*(const uint32_t& other) const;
		UBigInt2048 operator*(const UBigInt2048& other) const;

	private:
		uint32_t b[64];
	};
}
