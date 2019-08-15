#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void handler(int signum, siginfo_t *siginfo, void *wat)
{
	fputs("test failed!\n", stderr);
}

int main()
{
	sigset_t set;
	sigemptyset(&set);
	struct sigaction act;
	act.sa_sigaction = handler;
	act.sa_mask = set;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGABRT, &act, 0);
	abort();
}
