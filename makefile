# (c) 2015 Wojciech A. Koszek <wojciech@koszek.com>
SRCS_ALL:=$(wildcard *.c *.cc)
SRCS_EXCLUDED=
SRCS=$(filter-out $(SRCS_EXCLUDED),$(SRCS_ALL))
OBJECTS+=$(SRCS:.c=.pc)
OBJECTS+=$(SRCS:.cc=.pcpp)
CFLAGS+=-Wall -pedantic -g -ggdb
CXXFLAGS+=$(CFLAGS)

all:	$(OBJECTS)

%.pc: %.c
	$(CC) $(CFLAGS) $< -o $@
%.pcpp: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@ -lncurses

clean:
	rm -rf *.pc *.pcpp *.dSYM
