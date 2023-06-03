PYTHON_VENV=venv/bin/activate
ACTIVATE_VENV=. $(PYTHON_VENV)

ifeq ($(origin VIRTUAL_ENV), undefined)
    $(error Python virtual environment not active.)
endif

CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99
CTARGET=tubecpy
SRCS=src/main.c src/utility.c
OBJS=$(SRCS:.c=.o)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CLIBS=`pkg-config --cflags --libs gtk+-3.0`
else ifeq ($(UNAME_S),Darwin)
    CLIBS=`pkg-config --cflags --libs gtk+-3.0` -framework Cocoa
else
    $(error Unsupported operating system: $(UNAME_S))
endif

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@ $(CLIBS)

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(CTARGET) $(CLIBS)

.PHONY: clean

clean:
	$(RM) $(CTARGET) $(OBJS)
