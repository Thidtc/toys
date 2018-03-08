CFLAGS=-g

all: map.o hash_func.o

test_map: map.o hash_func.o test_map.c
	gcc ${CFLAGS} -o $@ $^

test_alloc: alloc.o test_alloc.c
	gcc ${CFLAGS} -o $@ $^

map.o: map.c map.h
	gcc ${CFLAGS} -c $^

hash_func.o: hash_func.c hash_func.h
	gcc ${CFLAGS} -c $^

alloc.o: alloc.c alloc.h
	gcc ${CFLAGS} -c $^

clean:
	rm -rf test_map test_alloc *.o
