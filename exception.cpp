#include <windows.h>
#include <iostream>
using namespace std;

class CommonException {
public:
	CommonException(int code, const string& msg="") : m_ErrCode(code), m_ErrMsg(msg) { }
	~CommonException() { }

	virtual int getCode() { return m_ErrCode; }
	virtual string getMessage() { return m_ErrMsg; }
	
private:
	int m_ErrCode;
	string m_ErrMsg;
};

class CalcException : public CommonException {
public:
	CalcException(int num, int code, const string& msg="") : m_Num(num), CommonException(code, msg) { }
	~CalcException() { }

private:
	int m_Num;
};

void TestFunc() 
{
	int a = 10;
	//a = a / 0;
	throw CalcException(1, 10, "calc error !!");
}

class TestClass {
public:
	TestClass() { }
	~TestClass() { }

	void show() {
		TestFunc();
		throw CommonException(10, "common error !!");
	}
};

int main()
{
	try {
		TestClass tc;
		tc.show();

		throw CommonException(20, "in main");

	} catch (CalcException &e) {
		cout << "CalcException." << endl;
		cout << e.getMessage().c_str() << endl;
	
	} catch (CommonException &e) {
		cout << "CommonException." << endl;
		cout << e.getMessage().c_str() << endl;

	} catch (...) {
		cout << "..." << endl;

	}
	
	//cout << "test exception.." << endl;

	return 0;
}

