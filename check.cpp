#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

void myfunction (string s) {
  cout << s << endl;
}

struct myclass {
  void operator() (string s) { cout << s << endl; }
} myobject;


int main()
try {
	vector<string> notes(100);
	vector<string>::iterator it;

	for (it = notes.begin(); it != notes.end(); it++)
	{
		*it = "test string";
	}

	for_each(notes.begin(), notes.end(), myobject);
	
	cout << notes.at(101) << endl;
	cout << notes[1001] << endl;

	return 0;
}
catch (out_of_range) {
	cerr << "range error\n";
}
catch (...) {
	cerr << "unknown exception thrown\n";
}

