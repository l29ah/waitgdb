#include <signal.h>
#include <sys/types.h>

void handler(int signum, siginfo_t *siginfo, void *wat)
{
	kill(0, SIGSTOP);
}

void install_sighandlers(void)
{
	sigset_t set;
	sigemptyset(&set);
	struct sigaction act;
	act.sa_sigaction = handler;
	act.sa_mask = set;
	act.sa_flags = SA_SIGINFO;

	sigaction(SIGABRT, &act, 0);
	sigaction(SIGBUS, &act, 0);
	sigaction(SIGFPE, &act, 0);
	sigaction(SIGILL, &act, 0);
	sigaction(SIGSEGV, &act, 0);
}
