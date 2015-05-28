#include <iostream>
#include <list>
#include <vector>
#include <string>
using namespace std;

void ParseKeyPath(const wstring& wpath, list<wstring>& wlist);
int FindKeyPath(const wstring& wpath, const list<wstring>& wlist);


int main()
{
	vector<wstring> wtest;
	wtest.push_back(L"\\NPKI\\KICA\\USER\\cn=kjh1,ou=RACenter,ou=ibk,ou=test RA,ou=licensedCA,o=KICA,c=KR");
	wtest.push_back(L"\\NPKI\\yessign\\USER\\cn=kks()001104220080113111001473,ou=NACF,ou=personal4IB,o=yessign,c=kr");
	wtest.push_back(L"\\NPKI\\yessign\\USER\\cn=hgd()0003041201312240956476,ou=IBK,ou=personal4IB,o=yessign,c=kr");

	vector<wstring> wfind;
	wfind.push_back(L"\\");
	wfind.push_back(L"\\NPKI");
	wfind.push_back(L"\\NPKI\\yessign\\USER");
	wfind.push_back(L"\\NPKI\\yessign\\USER\\cn=kks()001104220080113111001473,ou=NACF,ou=personal4IB,o=yessign,c=kr");

	vector<list<wstring>> lists;
	wstring delim = L"\\";

	// parse
	for (unsigned int i = 0; i < wtest.size(); i++) {
		list<wstring> templist;
		wstring& rt = wtest.at(i); 

		int start = delim.length();
		int end = rt.find(delim, 1);
		while (end != std::string::npos)
		{
			templist.push_back(rt.substr(start, end - start));
			start = end + delim.length();
			end = rt.find(delim, start);
		}
		templist.push_back(rt.substr(start));
		lists.push_back(templist);
	}


	// out 
	for (unsigned int i = 0; i < lists.size(); i++) {
		wstring wall = L"";

		list<wstring>& ot = lists.at(i); 
		for (list<wstring>::iterator it = ot.begin(); it != ot.end(); ++it) {
			wcout << *it << ":" << (*it).size() << endl;
			wall += *it;
		}

		if (wall == wtest.at(i)) {
			wcout << wall << endl << endl;
		}
	}

	// findchild
	// in : L"\\NPKI"
	// out : vector<wstring> subs
	/*
	wstring testin = L"\\NPKI";
	for (unsigned int i = 0; i < lists.size(); i++) {
		list<wstring>& rwl = lists.at(i);

		//int depth = FindKeyPath(testin, rwl);


		// FillFindData
	}
	*/


	return 0;
}

int FindKeyPath(const wstring& wpath, const list<wstring>& wlist)
{
	int depth = 0;

	list<wstring> templist;
	ParseKeyPath(wpath, templist);

	list<wstring>::const_iterator it_dir = wlist.begin();
	for (list<wstring>::iterator it = templist.begin(); it != templist.end(); ++it, ++it_dir) {
		if (*it != *it_dir) {
			return 0;
		}
		++depth;
	}

	return depth;
}

void ParseKeyPath(const wstring& wpath, list<wstring>& wlist)
{
    wstring delim = L"\\";

    auto start = 0U;
    auto end = wpath.find(delim, 1);
    while (end != std::string::npos)
    {
        wlist.push_back(wpath.substr(start, end - start));
        start = end; // + delim.length();
        end = wpath.find(delim, start + 1);
    }
    wlist.push_back(wpath.substr(start));
}
