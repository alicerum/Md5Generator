#include <iostream>

#include "../headers/ByteString.h"
#include "../headers/MD5File.h"
#include "../headers/MD5FileException.h"

using namespace std;

int main (int argc, char **argv)
{
	if (argc != 2) {
		cout << "Invalid arguments." << endl
				<< "Usage: Md5Generator <file path>" << endl;
		return 0;
	}

	try {
		cout << MD5File(argv[1]).computeMd5() << endl;
	} catch (MD5FileException &e) {
		cout << e.getMessage() << endl;
	}

	return 0;
}
