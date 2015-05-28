#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>

using namespace std;

class SecurePassword
{
public:
	SecurePassword() {}
	SecurePassword(SecurePassword* p) {}
	~SecurePassword() {}

	SecurePassword* get() { return this; }
	void test() { cout << "test.." << endl; }
};
typedef boost::shared_ptr<SecurePassword> SecurePasswordPtr;

void testfunc(SecurePasswordPtr p, SecurePassword& o);

int main()
{
	SecurePasswordPtr ptr(new SecurePassword());

	{
		cout << ptr.use_count() << endl;

		SecurePasswordPtr ptr2 = ptr;
		SecurePasswordPtr ptr3(ptr2);
		//SecurePassword& ptr3 = (*ptr);
		//SecurePassword* ptr4 = &(*ptr);

		//SecurePassword obj = ptr->get();
		//SecurePassword* pObj = &(*ptr);

		cout << ptr.use_count() << endl;

		testfunc(ptr, *(ptr));
	}

	cout << ptr.use_count() << endl;

	return 0;
}

void testfunc(SecurePasswordPtr p, SecurePassword& o)
{
	o.test();
	cout << p.use_count() << endl;
}

