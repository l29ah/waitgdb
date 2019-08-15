#include <stdlib.h>
#include "waitgdb.h"

int main()
{
	waitgdb_install_sighandlers();
	abort();
}
