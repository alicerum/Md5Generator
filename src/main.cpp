#include <iostream>
#include "../headers/ByteString.h"

using namespace std;

int main (int argc, char **argv)
{
	string a = "Lolka hello!";

	ByteString bStr;
	bStr.fromAnsi(a);

	cout << bStr.toHex() << endl;

	return 0;
}
