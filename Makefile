GCOV_OUTPUT = *.gcda *.gcno *.gcov 
GCOV_CCFLAGS = -fprofile-arcs -ftest-coverage
CC     = gcc
CCFLAGS = -g -Werror -W -fno-omit-frame-pointer -fno-common -fsigned-char $(GCOV_CCFLAGS)


all: tests

main.c:
	sh make-tests.sh > main.c

tests: main.c avl_tree.o test_avl_tree.c CuTest.c main.c
	$(CC) $(CCFLAGS) -o $@ $^
	./tests
	gcov main.c test_avl_tree.c avl_tree.c

avl_tree.o: avl_tree.c
	$(CC) $(CCFLAGS) -c -o $@ $^

clean:
	rm -f main.c avl_tree.o tests $(GCOV_OUTPUT)
