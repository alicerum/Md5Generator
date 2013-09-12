#pragma once

#include <string>

using std::string;

class MD5FileException
{
	string message_;

public:
	MD5FileException(string message);
	const string getMessage();
};
