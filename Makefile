CFLAGS := -Wall -Wextra -Wpedantic -std=c23

rawvec: rawvec.c rawvec.h 
	${CC} ${CFLAGS} -g -DVECTOR_TEST_MAIN -o $@ $<

clean:
	rm -fv rawvec

.PHONY: clean

