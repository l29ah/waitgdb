#include <stdlib.h>
#include "waitgdb.h"

int main()
{
	install_sighandlers();
	abort();
}
