#pragma once

#include <string>

using std::string;

class MD5File
{
	string filePath_;
	FILE *f_;
	int fileLength_;

protected:
	int getBytesToAppend();
	void doPaddings(unsigned char *buf, int len);

	void round1();
	void round2();
	void round3();
	void round4();

public:

	MD5File(string filePath);
	~MD5File();

	string computeMd5();
};

