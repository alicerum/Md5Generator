#pragma once

#include <string>

using std::string;

class MD5File
{
	string filePath_;
	FILE *f_;
	int fileLength_;

	uint32_t a_;
	uint32_t b_;
	uint32_t c_;
	uint32_t d_;

	uint32_t t_[64];

protected:
	int getBytesToAppend();
	void doPaddings(unsigned char *buf, int len);

	void initVector();

	void round(uint *x);

public:

	MD5File(string filePath);
	~MD5File();

	string computeMd5();
};

