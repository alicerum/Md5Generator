#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "../headers/ByteString.h"
#include "../headers/MD5File.h"
#include "../headers/MD5FileException.h"

#include "../headers/defines.h"

MD5File::MD5File(string filePath)
{
	this->filePath_ = filePath;
	f_ = fopen(filePath.c_str(), "rb");

	if (!f_)
		throw MD5FileException("Invalid file or file path");

	fseek(f_, 0, SEEK_END);
	fileLength_ = ftell(f_);
	fseek(f_, 0, SEEK_SET);

	initVector();
}

MD5File::~MD5File()
{
	if (f_)
		fclose(f_);
}

string MD5File::computeMd5()
{
	int bytesToAppend = getBytesToAppend();

	unsigned char buf[64];

	initVector();

	int blocksAmount = (fileLength_ + bytesToAppend) / 64 + 1;
	for (int i = 0; i < blocksAmount; i++) {
		memset(buf, 0, 64);

		size_t readen = fread(buf, 64, 64, f_);
		if (!readen)
			readen = fileLength_;

		if (!readen && i != blocksAmount - 1)
			throw MD5FileException("Wrong file length, file is probably corrupted");

		if (readen < 64 && i < blocksAmount - 2)
			throw MD5FileException("Wrong file length, file is probably corrupted");

		// do the necessary paddings
		doPaddings(buf, readen);

		// prepare 16 32-bit messages from the block
		// looks like every block will be divided in two halves and run one after another
		uint32_t x[16];
		for (int i = 0; i < 16; i++) {
			x[i] = buf[i*2] << 8 | buf[i*2+1];
		}

		// You spin me right round, baby
		// right round like a record, baby
		// Right round round round

		round(x);

		// another half of the block
		for (int i = 16; i < 32; i++) {
			x[i] = buf[i*2] << 8 | buf[i*2+1];
		}
		round(x);
	}

	ByteString bsA; bsA.fromUint(a_);
	ByteString bsB; bsA.fromUint(b_);
	ByteString bsC; bsA.fromUint(c_);
	ByteString bsD; bsA.fromUint(d_);

	return (bsA + bsB + bsC + bsD).toHex();
}

int MD5File::getBytesToAppend()
{
	// 512 bits
	int a = fileLength_ % 64;

	// 448 bits
	if (a < 56) {
		return 56 - a;
	}

	// append 64-a bytes to finish the last block and then append 56 bytes
	// to get 448 bits in a block
	return 120 - a;
}

void MD5File::doPaddings(unsigned char *buf, int len)
{
	if (len < 64) {
		int minLim = len;
		if (len) {
			buf[len] = 0x80;
			minLim++;
		}

		int maxLim = len < 56 ? 56 : 64;
		int j = 0;
		for (j = minLim; j < maxLim; j++) {
			buf[j] = 0x00;
		}

		// time to append file length in bits!
		if (j < 64) {
			uint64_t fileSizeInBits = fileLength_ * 8;

			// dirty hack <3
			for (int k = 0; k < 8; k++)
				buf[j+k] = fileSizeInBits >> k*8;
		}
	}
}

void MD5File::initVector()
{
	a_ = 0x67452301;
	b_ = 0xefcdab89;
	c_ = 0x98badcfe;
	d_ = 0x10325476;
}

void MD5File::round(uint32_t *x)
{
	uint32_t a = a_, b = b_, c = c_, d = d_;

	// round 1
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	// round 2
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	// round 3
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	// round 4
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	a_ += a;
	b_ += b;
	c_ += c;
	d_ += d;
}
