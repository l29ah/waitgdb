CFLAGS+=-std=gnu11 -D_GNU_SOURCE -fPIC
LDFLAGS+=-ldl
DESTDIR+=/usr/local/
ifdef DEBUG
	CFLAGS+=-ggdb -DDEBUG -O0 -Wall -pedantic
else
	CFLAGS+=-DNDEBUG
endif

all: test test_standalone waitgdb.so astyle

test: test.o waitgdb.o

waitgdb.so: waitgdb.o
	$(CC) $(CFLAGS) $(LDFLAGS) waitgdb.o -o waitgdb.so -shared

.PHONY: clean
clean:
	rm -rf *.o test test_standalone waitgdb.so

astyle:
	astyle --style=linux --indent=tab *.c *.h

.PHONY: install
install:
	cp waitgdb.so $(DESTDIR)/lib/

.PHONY: uninstall
uninstall:
	rm waitgdb.so $(DESTDIR)/lib/waitgdb.so
