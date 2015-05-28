#include <iostream>
#include <stdio.h>
using namespace std;

class CTest {
public:
	CTest() { }
	~CTest() { }

	static void GetInfo() { m_bAccessed = true; }
	bool IsAccessed() { return m_bAccessed; }

private:
	static bool m_bAccessed;
};

bool CTest::m_bAccessed;

int main()
{
	CTest ct;
	
	//ct.GetInfo();
	if (ct.IsAccessed()) {
		cout << "accessed.." << endl;
	} else {
		cout << "not yet.." << endl;
	}

	return 0;
}
