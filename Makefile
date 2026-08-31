CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic

keyboard-listener: keyboard-listener.c key-state.h key-side.h
	$(CC) $(CFLAGS) -o $@ $<

test: keyboard-side-test
	./keyboard-side-test

keyboard-side-test: tests/key-side-test.c key-state.h key-side.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f keyboard-listener keyboard-side-test

.PHONY: clean test
