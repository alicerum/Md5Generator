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

public:

	MD5File(string filePath);
	~MD5File();

	string computeMd5();
};

