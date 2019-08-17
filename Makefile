CFLAGS+=-std=gnu11 -D_GNU_SOURCE -fPIC
LDFLAGS+=-ldl
ifdef DEBUG
	CFLAGS+=-ggdb -DDEBUG -O0 -Wall -pedantic
else
	CFLAGS+=-DNDEBUG
endif

PRODUCTS=test test_standalone waitgdb.so
prefix=/usr/local

.PHONY: all install clean

all: $(PRODUCTS) astyle

install: waitgdb.so
	install waitgdb.so $(prefix)/lib/

test: test.o waitgdb.o

waitgdb.so: waitgdb.o
	$(CC) $(CFLAGS) $(LDFLAGS) waitgdb.o -o waitgdb.so -shared

clean:
	rm -rf *.o $(PRODUCTS)

astyle:
	astyle --style=linux --indent=tab *.c *.h
