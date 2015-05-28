#include <stdio.h>
#include <string>
#include <stdexcept>
using namespace std;

int main()
{
	try {
		printf("throw....\n");
		throw std::runtime_error("basic exception");
	} catch (std::exception& ex) {
		printf("In catch handler\n");
	}

	return 0;
}
