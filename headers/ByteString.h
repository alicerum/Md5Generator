#pragma once

#include <string>
#include <memory>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

using std::string;
using std::auto_ptr;
using std::unique_ptr;

class ByteString
{
	int len_;
	unique_ptr<char[]> ptr_;

public:
	ByteString(void);
	ByteString (const ByteString &bs);
	ByteString (const string &ansi);

	~ByteString(void);

	void fromHex(const string &hex);
	void fromAnsi(const string &ansi);
	void fromRaw(int len, char *raw);

	string toHex();
	string toAnsi();

	ByteString operator^(const ByteString &bs) const;
	ByteString operator+(const ByteString &bs) const;
	ByteString &operator=(const ByteString &bs);
	bool operator==(const ByteString &bs) const;

	int length();
	const char *ptr();
};
