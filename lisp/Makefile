CFLAGS=-g -rdynamic -Ilib
LINKFLAGS=-ldl

test_lispy: lisp.tab.c lex.yy.c test_lisp.c lispy.c builtin_func.c lispy.h\
	builtin_func.h lib/map.o lib/hash_func.o
	gcc ${CFLAGS} -o $@ $^ ${LINKFLAGS}

lispy: lisp.tab.c lex.yy.c parse.c lispy.c builtin_func.c lispy.h\
	builtin_func.h repl.o lib/map.o
	gcc ${CFLAGS} ${LINKFLAGS} -o $@ $^

repl.o: repl.c
	gcc ${CFLAGS} -leditline -c $^ -o $@

lisp.tab.h lisp.tab.c: lisp.y
	bison -d $^

lex.yy.c: lisp.l lisp.tab.h
	flex -i $^

lib/map.o: lib/map.c lib/map.h
	make -C ./lib

clean:
	rm -rf *.o lisp.tab.* test_lispy lispy
	make -C ./lib clean
