#include <windows.h>
#include <stdio.h>
#include <signal.h>

sig_atomic_t signaled = 0;

#include <time.h>
void PrintMessage(const char* msg)
{
    time_t timer;
    struct tm* tmlog;
    time(&timer);
    tmlog = localtime(&timer);

    char szTime[26] = {0, };
    sprintf(szTime, "%d%d%d %.2d:%.2d:%.2d",
            1900 + tmlog->tm_year, tmlog->tm_mon, tmlog->tm_mday,
            tmlog->tm_hour, tmlog->tm_min, tmlog->tm_sec);

    FILE* pFile = NULL;
    pFile = fopen("sigtest.log", "a+");
    if (pFile != NULL)
    {
        fprintf(pFile, "%s  %s\n", szTime, msg);
        fclose(pFile);
    }
}

void handler_ABRT(int param)
{
	signaled = 1;
	PrintMessage("handler_ABRT: SIGABRT");
}

void handler_FPE(int param)
{
	signaled = 1;
	PrintMessage("handler_FPE: SIGFPE");
}

void handler_ILL(int param)
{
	signaled = 1;
	PrintMessage("handler_ILL: SIGILL");
}

void handler_INT(int param)
{
	signaled = 1;
	PrintMessage("handler_INT: SIGINT");
}

void handler_SEGV(int param)
{
	signaled = 1;
	PrintMessage("handler_SEGV: SIGSEGV");
}

void handler_TERM(int param)
{
	signaled = 1;
	PrintMessage("handler_TERM: SIGTERM");
}


int main()
{
	void (*fn_handler_abort)(int);
	void (*fn_handler_fpe)(int);
	void (*fn_handler_illegal)(int);
	void (*fn_handler_interrupt)(int);
	void (*fn_handler_segmentation)(int);
	void (*fn_handler_terminate)(int);

	fn_handler_abort = signal(SIGABRT, handler_ABRT);
	fn_handler_fpe = signal(SIGABRT, handler_FPE);
	fn_handler_illegal = signal(SIGABRT, handler_ILL);
	fn_handler_interrupt = signal(SIGABRT, handler_INT);
	fn_handler_segmentation = signal(SIGABRT, handler_SEGV);
	fn_handler_terminate = signal(SIGABRT, handler_TERM);

	while(1) {
		PrintMessage("signal wait...");
		Sleep(1000);
		if (signaled == 1) {
			PrintMessage("signal received...");
		}
	}

	return 0;
}


