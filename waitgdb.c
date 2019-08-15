#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <dlfcn.h>

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

	sigaction(SIGABRT, &act, 0);
	sigaction(SIGBUS, &act, 0);
	sigaction(SIGFPE, &act, 0);
	sigaction(SIGILL, &act, 0);
	sigaction(SIGSEGV, &act, 0);
}

/* Trampoline for the real main() */
static int (*main_orig)(int, char **, char **);

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

	/* ... and call it with our custom main function */
	return orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end);
}
