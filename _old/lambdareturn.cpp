#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>
using namespace std;
 
int main() {
    vector<int> v;
 
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }
 
    deque<double> d;
 
    transform(v.begin(), v.end(), front_inserter(d), [](int n) -> double {
        if (n % 2 == 0) {
            return n * n * n;
        } else {
            return n / 2.0;
        }
    });
 
    for_each(d.begin(), d.end(), [](double x) { cout << x << " "; });
    cout << endl;
}

