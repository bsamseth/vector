CFLAGS := -Wall -Wextra -Wpedantic -std=c23

test: testmain
	./testmain

valgrind: testmain
	valgrind ./testmain

rawvec: rawvec.c rawvec.h 
	${CC} ${CFLAGS} -g -DRAWVEC_TEST_MAIN -o $@ $<

testmain: testmain.c rawvec.c vector.h rawvec.h unity.c
	${CC} ${CFLAGS} -g -o $@ $^

clean:
	rm -fv rawvec testmain

.PHONY: clean

