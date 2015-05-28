#include <windows.h>
#include <stdio.h>
#include "SafeHardHSM.h"

void CheckArgs(void* void_p_args);

int main()
{
	SAFEHARD_ARGS init_args = { "unknown", "unknown", "http://cloud.safehard.co.kr/safeHardRelayServer/safeHardReq.do" };

	CheckArgs(&init_args);

	return 0;
}

void CheckArgs(void* void_p_args)
{
	P_SAFEHARD_ARGS p_args = (P_SAFEHARD_ARGS)void_p_args;

	printf("args size: %d\n", sizeof(*p_args));
	printf("SAFEHARD_ARGS size: %d\n", sizeof(_safehard_args));

	printf("domain name: %s\n", p_args->szDomainName);
	printf("company name: %s\n", p_args->szCompanyName);
	printf("cloud server url: %s\n", p_args->szCloudServerURL);

	SAFEHARD_ARGS args = *p_args;

	printf("args domain name: %s\n", args.szDomainName);
	printf("args company name: %s\n", args.szCompanyName);
	printf("args cloud server url: %s\n", args.szCloudServerURL);
}

