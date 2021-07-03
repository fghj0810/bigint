#include <iostream>
#include <cstdio>

#include "bigint.hpp"

using namespace std;


void UBigInt2048_print(const bigint::BigInt<2048>& num)
{
	uint8_t buffer[256];
	num.getBytes(buffer);

	for (int i = 0; i < 256; i++)
	{
		printf("%02x", buffer[i]);
	}
	std::cout << std::endl;
}

int main()
{
	uint8_t aa[] = { 0xdb, 0xa3, 0x0a, 0x50, 0x8f, 0x46, 0x5f, 0xb1, 0x80, 0x4d, 0x68, 0xd0, 0x2d, 0xc5, 0x78, 0x16, 0x51, 0x16, 0xb7, 0xd4, 0x1c, 0x73, 0x76, 0x55, 0x98, 0xcd, 0xc8, 0xe1, 0x8b, 0xc2, 0x80, 0x4b, 0x3e, 0xa0, 0xda, 0x09, 0x5c, 0xc4, 0x09, 0x7b, 0x7c, 0x09, 0x2e, 0x1e, 0x85, 0x12, 0x59, 0xc0, 0x61, 0xb7, 0x0a, 0x8a, 0x85, 0x8f, 0xa8, 0xfa, 0x53, 0x47, 0xae, 0xd8, 0xa3, 0xa3, 0xe9, 0xba, 0x3e, 0xa0, 0x2b, 0x45, 0xee, 0x53, 0xb9, 0xdd, 0x8b, 0xad, 0xcf, 0x0b, 0xd7, 0xd7, 0x02, 0x3f, 0x7f, 0x35, 0xfb, 0x99, 0x06, 0x9b, 0x33, 0x44, 0x0f, 0x4d, 0xd6, 0xaf, 0x36, 0x37, 0x86, 0x35, 0x56, 0x62, 0xb5, 0x9b, 0x74, 0xcd, 0x48, 0x00, 0x70, 0xc5, 0xa5, 0xd2, 0x84, 0x40, 0x64, 0x24, 0x77, 0x1a, 0xe5, 0x0e, 0x47, 0x4c, 0x89, 0x32, 0x7b, 0x77, 0x0b, 0x41, 0xbb, 0xde, 0xd4, 0xf9, 0x66, 0x2d, 0xa7, 0x26, 0xaa, 0xb8, 0xc0, 0x83, 0xd0, 0x54, 0x47, 0x4b, 0xd7, 0x24, 0x0b, 0xe9, 0x1e, 0xa3, 0x18, 0x6d, 0x1d, 0xb9, 0x5a, 0x26, 0x69, 0x9b, 0x46, 0x74, 0xaf, 0x5b, 0x19, 0x62, 0x24, 0xd4, 0xd9, 0x4f, 0x8c, 0x1e, 0xed, 0x6b, 0x0a, 0xb1, 0xc2, 0xd4, 0x3a, 0xaa, 0x0b, 0xee, 0x27, 0xc8, 0x47, 0x7f, 0x85, 0xb1, 0xaf, 0x1a, 0x0f, 0xfd, 0x42, 0xec, 0x3f, 0x5c, 0x51, 0xe8, 0x60, 0xf7, 0x01, 0x32, 0x37, 0x1a, 0x32, 0x38, 0x28, 0x69, 0x72, 0x1e, 0x6b, 0x32, 0x97, 0x63, 0xa4, 0x9d, 0x88, 0xd5, 0x0b, 0x16, 0x00, 0xff, 0xca, 0xf5, 0xcb, 0x46, 0xac, 0x88, 0x27, 0xe9, 0x2f, 0xb1, 0x47, 0x54, 0xf2, 0x55, 0x89, 0x24, 0x51, 0x81, 0xf6, 0x45, 0x1a, 0x45, 0x8a, 0x81, 0x77, 0x60, 0xff, 0x8e, 0x91, 0x95, 0xdd, 0xcb, 0x47, 0x3b, 0xa1, 0x5a, 0xc9, 0xa4, 0x84, 0x33 };
	uint8_t bb[] = { 0xae, 0x9d, 0x6d, 0x8a, 0xe4, 0x8d, 0x77, 0x70, 0xbf, 0xe7, 0x4b, 0x08, 0x0f, 0x0b, 0xaa, 0x50, 0x5b, 0xc1, 0x6a, 0xfe, 0xb8, 0x6b, 0xd7, 0x4d, 0x70, 0x01, 0x52, 0xce, 0x75, 0xf5, 0x79, 0x57, 0xd8, 0xff, 0xba, 0xb8, 0xac, 0xc6, 0xb2, 0x1f, 0x39, 0x5a, 0x77, 0x0f, 0x0e, 0x8e, 0xab, 0x4a, 0x36, 0xa5, 0x76, 0x3c, 0xf0, 0xb8, 0x69, 0x08, 0x33, 0x03, 0x8f, 0x6f, 0x6e, 0x91, 0xd7, 0x35, 0x19, 0xaa, 0xef, 0x1c, 0x56, 0xc0, 0xbb, 0xc8, 0xf5, 0xce, 0x86, 0x08, 0x0d, 0x5e, 0x43, 0xcc, 0x45, 0x8f, 0x9e, 0x6a, 0x67, 0x85, 0x25, 0xf9, 0xdb, 0x0c, 0xf0, 0x3c, 0x36, 0x27, 0x68, 0x8c, 0xa9, 0x14, 0x2a, 0xfb, 0xdf, 0x9d, 0x80, 0xbb, 0xd0, 0xec, 0x61, 0xcb, 0xff, 0x16, 0xce, 0x46, 0xeb, 0x66, 0x37, 0x6a, 0xe5, 0xa6, 0x64, 0x80, 0xfd, 0x05, 0x84, 0xae, 0x8a, 0xf5, 0x3a, 0x6d, 0x5f, 0x97, 0x11, 0xf8, 0xc6, 0x19, 0x6c, 0xb0, 0xfe, 0x19, 0xa5, 0x8b, 0xf6, 0x62, 0x73, 0xf8, 0xa0, 0x54, 0xa7, 0x5d, 0x2f, 0xf6, 0x21, 0x41, 0xdd, 0xc7, 0x9b, 0xc0, 0x37, 0x98, 0x11, 0x77, 0x43, 0x49, 0x85, 0xcf, 0xdf, 0x2d, 0x71, 0x5b, 0x68, 0x23, 0xa9, 0xd0, 0x38, 0x0d, 0xb2, 0x74, 0x49, 0x8a, 0xa8, 0x7a, 0x95, 0x19, 0x0d, 0xfe, 0x67, 0xe1, 0xca, 0x99, 0x44, 0x22, 0xd4, 0xb9, 0x08, 0x8b, 0x20, 0xfc, 0x3a, 0xfb, 0xd3, 0xed, 0xcc, 0xfb, 0x86, 0x81, 0x16, 0x27, 0x52, 0xb7, 0x63, 0x88, 0xee, 0x5d, 0x20, 0x66, 0x8d, 0x6f, 0x63, 0x16, 0x67, 0x3f, 0xa9, 0x12, 0xf4, 0xa7, 0x9d, 0x46, 0x4c, 0xcc, 0x76, 0xf7, 0x23, 0x48, 0x18, 0x1a, 0x1d, 0x3c, 0xc5, 0x50, 0x77, 0x06, 0x4c, 0x6f, 0x9b, 0x1a, 0xd1, 0x23, 0xe9, 0x2a, 0x6d, 0x84, 0xd2, 0x72, 0xd1, 0x61, 0x84, 0x91 };
	//uint8_t aa[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	//uint8_t bb[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	bigint::BigInt<2048> a(aa, 256);
	UBigInt2048_print(a);
	bigint::BigInt<2048> b(bb, 256);
	UBigInt2048_print(b);
	//UBigInt2048_print(a + b);
	//UBigInt2048_print(a - b);
	//UBigInt2048_print(bigint::BigInt<2048>() - (b - a));
	//UBigInt2048_print(a * 0);
	//UBigInt2048_print(a * 1);
	//UBigInt2048_print(a * 16);
	//UBigInt2048_print(a * 0xFFFFFFFF);
	//UBigInt2048_print((a << 32) - a);
	UBigInt2048_print(a * b);
	//UBigInt2048_print(a / b);
	//UBigInt2048_print(a >> 1024);
	//UBigInt2048_print(a << 1024);
	//UBigInt2048_print(a >> 1020);
	//UBigInt2048_print(a << 1020);
	//UBigInt2048_print(a >> 1028);
	//UBigInt2048_print(a << 1028);
	//UBigInt2048_print(a >> 4);
	//UBigInt2048_print(a << 4);
	//UBigInt2048_print(a >> 2044);
	//UBigInt2048_print(a << 2044);
	UBigInt2048_print(a / (bigint::BigInt<2048>() + 1));
	UBigInt2048_print(a / b);
	UBigInt2048_print(b / a);
	std::cout << sizeof(bigint::BigInt<2048>) << std::endl;
	//uint32_t u = 0x12345678;
	//std::cout << std::hex << (u << -8) << std::endl;
	return 0;
}