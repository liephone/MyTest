#include <windows.h>
#include <stdio.h>
#include "Duration.h"

class TestClass 
{
public:
	TestClass() {}
	~TestClass() {}

	void ShowTime() { 
		m_timer.Stop();
		printf("TestClass's time is [%lf]\n", m_timer.GetDuration() / 1000.0); 
	}
	void Reset() { m_timer.Start(); }

private:
	CDuration m_timer;
};

int main()
{
	CDuration timer;
	timer.Start();
	TestClass tc;

	CDuration* pTimer;
	pTimer = new CDuration();
	pTimer->Start();

	getchar();

	timer.Stop();
	printf("timer is [%lf] ms\n", timer.GetDuration() / 1000.0);

	Sleep(500);

	timer.Stop();
	printf("timer is [%lf] ms\n", timer.GetDuration() / 1000.0);

	timer.Start();
	Sleep(100);
	timer.Stop();
	printf("timer is [%lf] ms\n", timer.GetDuration() / 1000.0);

	tc.ShowTime();
	tc.Reset();
	tc.ShowTime();

	return 0;
}

