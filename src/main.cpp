#include <iostream>

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

	cout << "Counting md5 for file '" << argv[1] << "'" << endl;

	try {
		cout << MD5File(argv[1]).computeMd5() << endl;
	} catch (MD5FileException &e) {
		cout << e.getMessage() << endl;
	}

	return 0;
}
