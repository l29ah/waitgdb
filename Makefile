CFLAGS+=-std=c11 -D_POSIX_C_SOURCE=199309L
ifdef DEBUG
	CFLAGS+=-ggdb -DDEBUG -O0 -Wall -pedantic
else
	CFLAGS+=-DNDEBUG
endif

test: test.o waitgdb.o
