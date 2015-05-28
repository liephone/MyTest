#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

// const value, const pointer
const char* const authorName = "Scott Meyers";
const std::string editorName = "Scott Meyers";

class GamePlayer
{
	private:
		//static const int NumTurns = 5;
		enum { NumTurns = 5 };
		enum MODE { VERIFY_IGNORE = 0, VERIFY_MUST, VERIFY_CONFIRM, VERIFY_END };
		int scores[NumTurns];
		string m_Name;
	public:
		MODE TestGetMode() const { return VERIFY_MUST; }
};

class TestClass
{
private:
	vector<string> m_codes;

public:
	TestClass() { m_codes.push_back("test..."); }
	~TestClass() {}

	vector<string>& GetCodes() { return m_codes; }

};

int main()
{
	// test...
	TestClass tc;
	vector<string>& codes = tc.GetCodes();

	codes.push_back("const test...");

	cout << "===" << endl;
	for (int i = 0; i < codes.size(); i++) {
		cout << codes.at(i).c_str() << endl;
	}


	// integer
	int a = 1;
	const int b = 2;

	int* pa = &a;
	const int* pb = &b;

	a = b;
	//pa = pb;		// error
					// const -> non const : value ok, pointer not ok

	// character array
	char arA[10] = "array 1";
	const char arB[10] = "array 2";
	char arC[10] = "array C";
	
	//arA = arB;	// error
	//arB = arA;	// error
	// char * strcpy ( char * destination, const char * source );
	int p1 = (int)(&arA[0]);
	int p2 = (int)(&arB[0]);

	char* szA = "test 1";
	const char* szB = "test 2";
	char* szC = "test temp";

	//szA = szB;	// error
	szA = szC;

	
	// class
	string strA("string 1");
	const string strB("string 2");
	string strC(szA);
	const string strD(szB);

	strA = strB;	// no error
	strC = strD;	// no error

	cout << a << endl;
	cout << arA << endl;
	cout << szA << endl;
	cout << *pa << endl;
	cout << strA << endl;
	cout << &strA << endl;
	cout << &strB << endl;
	

	// STL iterator
	//std::vector<int> vec;

	//const std::vector<int>::iterator iter = vec.begin();
	//*iter = 10;			// ok
	//++iter;			// error
	
	//std::vector<int>::const_iterator cIter = vec.begin();
	//*cIter = 10;		// error
	//++cIter;			// ok

	return 0;
}


// const function 
class TextBlock
{
public:
	//...

	// case 1. ... but code ม฿บน
	//const char& operator[] (std::size_t position) const		// can override with const..
	//{ return text[position]; }

	//char& operator[] (std::size_t position)
	//{ return text[position]; }

	// case 2. ..
	const char& operator[] (std::size_t position) const
	{ return text[position]; }

	char& operator[] (std::size_t position) 
	{ return const_cast<char&> (static_cast<const TextBlock&>(*this)[position]); }
	
private:
	std::string text;
};

