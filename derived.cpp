#include <iostream>
using namespace std;

class BaseClass 
{
public:
	BaseClass() { cout << "BaseClass construction" << endl; }
	virtual ~BaseClass() { cout << "BaseClass destruction" << endl; }
};

class AClass : BaseClass
{
public:
	AClass() { cout << "AClass construction" << endl; }
	~AClass() { cout << "AClass destruction" << endl; }

	void show() { cout << "AClass show message" << endl; }
};

class BClass : BaseClass
{
public:
	BClass() { cout << "BClass construction" << endl; }
	~BClass() { cout << "BClass destruction" << endl; }

	void show() { cout << "BClass show message" << endl; }
};

int main()
{
	BaseClass* pBase = NULL;
	AClass* pA = NULL;

	//pBase = (BaseClass*)(new AClass());
	//((AClass*)pBase)->show();

	//delete pBase;
	
	pA = <dynamic_cast>(AClass*)(new BaseClass());
	delete pA;

	return 0;
}
