.PHONY: all check clean

all: check

check:
	${MAKE} -C src all
	${MAKE} -C tests check

clean:
	${MAKE} -C src clean
	${MAKE} -C tests clean
