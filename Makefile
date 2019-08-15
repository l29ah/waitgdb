CFLAGS+=-std=gnu11 -D_GNU_SOURCE -fPIC
LDFLAGS+=-ldl
ifdef DEBUG
	CFLAGS+=-ggdb -DDEBUG -O0 -Wall -pedantic
else
	CFLAGS+=-DNDEBUG
endif

all: test waitgdb.so

test: test.o waitgdb.o

waitgdb.so: waitgdb.o
	$(CC) $(CFLAGS) $(LDFLAGS) waitgdb.o -o waitgdb.so -shared

clean:
	rm -rf *.o test waitgdb.so

astyle:
	astyle --style=linux --indent=tab *.c *.h
