TARGET=nodeviewd
INSTALL=_install
CC=gcc
MAKE=make
CFLAGS=-Iinclude -g -ggdb -O0
LDFLAGS=-L.
LIBS=-lpthread
SRCS=$(wildcard ./src/*.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

.PHONY: all
all: 	$(TARGET)

$(TARGET): $(OBJS)
	$(MAKE) -C src all
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: clean
clean:
	$(MAKE) -C src clean
	rm -rf $(TARGET)
	rm -rf $(INSTALL)

.PHONY: install
install: all
	mkdir -p $(INSTALL)
	cp -frv $(TARGET) $(INSTALL)
