DEBUG_OPTION=-g

pascal: main.o error.o scan.o parse.o utils.o
	gcc ${DEBUG_OPTION} -o $@ $^

utils.o: utils.c
	gcc ${DEBUG_OPTION} -c utils.c -o utils.o

error.o: error.c utils.h
	gcc ${DEBUG_OPTION} -c error.c -o error.o

main.o: main.c global.h utils.h pascal.tab.h 
	gcc ${DEBUG_OPTION} -c main.c -o main.o

parse.o: pascal.tab.c
	gcc ${DEBUG_OPTION} -c pascal.tab.c -o parse.o

scan.o: lex.yy.c
	gcc ${DEBUG_OPTION} -c lex.yy.c -o scan.o

pascal.tab.h pascal.tab.c: pascal.y
	bison -d $^

lex.yy.c: pascal.l pascal.tab.h
	flex -i $^

clean:
	rm *.o pascal.tab.*
