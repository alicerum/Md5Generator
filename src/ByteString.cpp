#include "../headers/ByteString.h"


ByteString::ByteString(void)
{
	len_ = 0;
}

ByteString::~ByteString(void)
{
}

ByteString::ByteString(const ByteString &bs)
{
	len_ = bs.len_;
	ptr_ = unique_ptr<char[]>(new char[len_]);
	memcpy((void*)ptr_.get(), (void*)bs.ptr_.get(), len_);
}

ByteString::ByteString(const string &ansi)
{
	fromAnsi(ansi);
}

void ByteString::fromHex(const string &hex)
{
	if (len_) {
		ptr_.release();
		len_ = 0;
	}

	len_ = hex.length() / 2;

	ptr_ = unique_ptr<char[]>(new char[len_]);

	for (unsigned int i = 0; i < hex.length(); i+=2) {
		char a[3];
		a[2] = 0;
		a[0] = hex[i];
		a[1] = hex[i+1];

		char *p;
		ptr_.get()[i/2] = (char)strtoul(a, &p, 16);
	}
}

void ByteString::fromRaw(int len, char *raw)
{
	if (len_ > 0)
		ptr_.release();

	len_ = len;
	ptr_ = unique_ptr<char[]>(new char[len_]);
	memcpy(ptr_.get(), raw, len_);
}

void ByteString::fromAnsi(const string &ansi)
{
	if (len_) {
		ptr_.release();
		len_ = 0;
	}

	len_ = ansi.length();
	ptr_ = unique_ptr<char[]>(new char[len_]);

	memcpy(ptr_.get(), ansi.c_str(), len_);
}

void ByteString::fromUint(unsigned int value)
{
	if (len_) {
		ptr_.release();
		len_ = 0;
	}

	len_ = 8;
	ptr_ = unique_ptr<char[]>(new char[len_]);

	for (int i = 0; i < 8; i++)
		ptr_.get()[i] = value >> i*8;
}

string ByteString::toHex()
{
	string output("");
	for (int i = 0; i < len_; i++) {
		unsigned char c = ptr_.get()[i];
		
		char a[3];
		a[2] = 0;

		sprintf(a, "%02x", c);

		output.append(a);
	}

	return output;
}

string ByteString::toAnsi()
{
	char *r = new char[len_ + 1];
	memcpy(r, ptr_.get(), len_);
	r[len_] = 0;

	string result(r);
	delete []r;

	return result;
}

ByteString ByteString::operator^(const ByteString &bs) const
{
	int len = this->len_;
	if (len > bs.len_)
		len = bs.len_;

	char *cRes = new char[len];
	for (int i = 0; i < len; i++)
		cRes[i] = ptr_.get()[i] ^ bs.ptr_.get()[i];

	ByteString newBS;
	newBS.len_ = len;
	newBS.ptr_ = unique_ptr<char[]>(cRes);

	return newBS;
}

ByteString ByteString::operator+(const ByteString &bs) const
{
	int newLen = len_ + bs.len_;
	char *newPtr = new char[newLen];

	memcpy(newPtr, ptr_.get(), len_);
	memcpy(newPtr + len_, bs.ptr_.get(), bs.len_);

	ByteString newBS;
	newBS.len_ = newLen;
	newBS.ptr_ = unique_ptr<char[]>(newPtr);

	return newBS;
}

int ByteString::length()
{
	return len_;
}

const char *ByteString::ptr()
{
	return ptr_.get();
}

ByteString &ByteString::operator=(const ByteString &bs)
{
	if (this != &bs) {
		this->len_ = bs.len_;
		this->ptr_ = unique_ptr<char[]>(new char[len_]);
		memcpy(ptr_.get(), bs.ptr_.get(), len_);
	}

	return *this;
}

bool ByteString::operator==(const ByteString &bs) const
{
	if (len_ != bs.len_)
		return false;

	return !memcmp(ptr_.get(), bs.ptr_.get(), len_);
}
