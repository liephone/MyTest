#include <iostream>
using namespace std;

class A
{
public:
	explicit A() { cout << "A()" << endl; }
	A(int x = 0, bool b = true) { cout << "A(int, bool)" << endl; }
	~A() { cout << "A dest.." << endl; }
};

class C
{
public:
    C(void) { cout << "C(void)" << endl; }
    C(int) { cout << "C(int)" << endl; }
	~C() { }
    //explicit C(void) { cout << "C(void)" << endl; }
};

int main(void)
{
    C c();
	A a();

    return 0;
}

