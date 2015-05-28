#include <iostream>
#include <string>
using namespace std;

class SaveKeyPair
{
public:
	SaveKeyPair();
	~SaveKeyPair();

	void save(string& name, const int flag = 0);
};

SaveKeyPair::SaveKeyPair()
{
}

SaveKeyPair::~SaveKeyPair()
{
}

void SaveKeyPair::save(string& name, const int flag)
{
	if (flag) 
		cout << name << ": flag is true" << endl;
	else 
		cout << name << ": flag is false" << endl;
}

int main()
{
	SaveKeyPair sk;
	string strName("test");

	sk.save(strName);
	sk.save(strName, 1);

	return 0;
}

