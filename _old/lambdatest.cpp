#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

int main()
{
	string text = "C++0x Lambda!";

	function<void()> lambda = [=]()
	{
		cout << text << endl;
	};

	vector< function<void()> > container;
	{
		container.push_back( lambda );
		container.push_back( [=] () { cout << text << endl; } );
	}

	for_each( container.begin(), container.end(), [](const function<void()>& f) { f(); });

	return 0;
}

