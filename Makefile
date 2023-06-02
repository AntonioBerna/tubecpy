PYTHON_VENV=venv/bin/activate
ACTIVATE_VENV=. $(PYTHON_VENV)

ifeq ($(origin VIRTUAL_ENV), undefined)
    $(error Python virtual environment not active.)
endif

CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99
CLIBS=`pkg-config --cflags --libs gtk+-3.0`
CTARGET=tubecpy
SRCS=src/main.c src/utility.c
OBJS=$(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@ $(CLIBS)

all: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(CTARGET) $(CLIBS)

.PHONY: clean

clean:
	$(RM) $(CTARGET) $(OBJS)
