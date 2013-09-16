#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "../headers/ByteString.h"
#include "../headers/MD5File.h"
#include "../headers/MD5FileException.h"

MD5File::MD5File(string filePath)
{
	this->filePath_ = filePath;
	f_ = fopen(filePath.c_str(), "rb");

	if (!f_)
		throw MD5FileException("Invalid file or file path");

	fileLength_ = fseek(f_, SEEK_END, 0);
	fseek(f_, SEEK_SET, 0);
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

	int blocksAmount = (fileLength_ + bytesToAppend) / 64 + 1;
	for (int i = 0; i < blocksAmount; i++) {

		ByteString bsBlock;
		memset(buf, 0, 64);

		int readen = fread(buf, 64, 64, f_);

		if (!readen && i != blocksAmount - 1)
			throw MD5FileException("Wrong file length, file is probably corrupted");

		if (readen < 64 && i < blocksAmount - 2)
			throw MD5FileException("Wrong file length, file is probably corrupted");

		// do the necessary paddings
		doPaddings(buf, readen);

		// You spin me right round, baby
		// right round like a record, baby
		// Right round round round

		round1();
		round2();
		round3();
		round4();
	}

	return string("");
}

int MD5File::getBytesToAppend()
{
	// 512 bits
	int a = fileLength_ % 64;

	// 448 bits
	if (a >= 56) {
		return a + 8;
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

void MD5File::round1()
{
}

void MD5File::round2()
{
}

void MD5File::round3()
{
}

void MD5File::round4()
{
}
