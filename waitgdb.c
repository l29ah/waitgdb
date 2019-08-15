#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <dlfcn.h>

// Trampolines for the real functions
static int (*main_orig)(int, char **, char **);
static typeof(&sigaction) sigaction_orig;
static const int my_signals[] = {
	SIGABRT,
	SIGBUS,
	SIGFPE,
	SIGILL,
	SIGSEGV,
};

static void handler(int signum, siginfo_t *siginfo, void *wat)
{
	kill(0, SIGSTOP);
}

void waitgdb_install_sighandlers(void)
{
	sigset_t set;
	sigemptyset(&set);
	struct sigaction act;
	act.sa_sigaction = handler;
	act.sa_mask = set;
	act.sa_flags = SA_SIGINFO;

	for (int i = 0; i < sizeof(my_signals); ++i) {
		sigaction_orig(my_signals[i], &act, 0);
	}
}

// forbid redefining our signals
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
	for (int i = 0; i < sizeof(my_signals); ++i) {
		if (signum == my_signals[i]) {
			errno = EACCES;
			return -1;
		}
	}
	return sigaction_orig(signum, act, oldact);
}

/* Our fake main() that gets called by __libc_start_main() */
int main_hook(int argc, char **argv, char **envp)
{
	waitgdb_install_sighandlers();
	int ret = main_orig(argc, argv, envp);
	return ret;
}

/*
 * Wrapper for __libc_start_main() that replaces the real main
 * function with our hooked version.
 */
int __libc_start_main(
    int (*main)(int, char **, char **),
    int argc,
    char **argv,
    int (*init)(int, char **, char **),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void *stack_end)
{
	/* Save the real main function address */
	main_orig = main;

	/* Find the real __libc_start_main()... */
	typeof(&__libc_start_main) orig = dlsym(RTLD_NEXT, "__libc_start_main");
	sigaction_orig = dlsym(RTLD_NEXT, "sigaction");

	/* ... and call it with our custom main function */
	return orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end);
}
