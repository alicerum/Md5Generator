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
	return string("");
}
