#include <stdio.h>

#include "../headers/MD5File.h"
#include "../headers/MD5FileException.h"

MD5File::MD5File(string filePath)
{
	this->filePath_ = filePath;
	f_ = fopen(filePath.c_str(), "r");

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
