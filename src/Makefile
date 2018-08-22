CC=gcc
CFLAGS=-I../include -g -ggdb -O0 -Wall -Werror -Wextra -pedantic
LDFLAGS=-L.
LIBS=-lpthread
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

.PHONY: all
all: 	$(OBJS)

$(DEPS): %.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $^

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(DEPS)
