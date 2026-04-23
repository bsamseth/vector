CFLAGS := -Wall -Wextra -Wpedantic -std=c23

test: testmain
	./testmain

valgrind: testmain
	valgrind ./testmain

testmain: testmain.c vector.h unity.h unity_internals.h unity.c
	${CC} ${CFLAGS} -g testmain.c unity.c -o $@

clean:
	rm -fv testmain

.PHONY: clean

