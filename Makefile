CFLAGS := -Wall -Wextra -Wpedantic -std=c23

test: testmain
	./testmain

valgrind: testmain
	valgrind ./testmain

testmain: testmain.c vector.h unity.h unity.c
	${CC} ${CFLAGS} -g -o $@ $^

clean:
	rm -fv testmain

.PHONY: clean

