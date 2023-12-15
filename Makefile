PROJDIR := ${CURDIR}

CORE_SRC := $(wildcard src/*.c)
TEST_SRC := $(wildcard test/*.c)
TEST_SO_SRC := $(wildcard test/so/*.c)

CFLAGS := -Wall -Werror -fPIC

CFLAGS += -I./

ifdef debug
CFLAGS += -ggdb3 -g3 -O0 -DSTATIC= -DDEBUG= 
BISONOPT := -rall
else
CFLAGS += -O2 -DSTATIC=static
BISONOPT :=
endif

GCC ?= gcc
RM := rm

a.out: $(TEST_SRC)
	@$(GCC) $(CFLAGS) -o $@ $^ -L./ -ltest -lsymbollookup

libsymbollookup.so: $(CORE_SRC)
	@$(GCC) $(CFLAGS) -shared -Wl,-E $^ -o $@ -lbfd

libtest.so: $(TEST_SO_SRC)
	@$(GCC) $(CFLAGS) -shared -Wl,-E $^ -o $@

clean: force
	rm -f a.out *.so tags

force:

# eof
