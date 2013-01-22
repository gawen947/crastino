include commands.mk

CFLAGS  := -std=c99 -O2 -fPIC -Wall $(pkg-config --cflags libconfuse gtk+-2.0)
LDFLAGS := $(pkg-config --libs libconfuse gtk+-2.0)

SRC  = $(wildcard *.c)
OBJ  = $(foreach obj, $(SRC:.c=.o), $(notdir $(obj)))
DEP  = $(SRC:.c=.d)

PREFIX  ?= /usr/local
BINDIR  ?= $(PREFIX)/bin

ifdef DEBUG
CFLAGS += -ggdb
endif

commit = $(shell ./hash.sh)
ifneq ($(commit), UNKNOWN)
CFLAGS += -DCOMMIT="\"$(commit)\""
endif

.PHONY: all clean

all: crastino

crastino: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -Wp,-MMD,$*.d -c $(CFLAGS) -o $@ $< 

clean:
	$(RM) $(DEP)
	$(RM) $(OBJ)
	$(RM) crastino

install: crastino
	$(INSTALL_PROGRAM) crastino $(BINDIR)

uninstall:
	$(RM) $(BINDIR)/crastino

-include $(DEP)

