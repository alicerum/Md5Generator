#include "../headers/MD5FileException.h"

MD5FileException::MD5FileException(string message)
{
	this->message_ = message;
}

const string MD5FileException::getMessage()
{
	return message_;
}
