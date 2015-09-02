CC = gcc
OBJ = AVL_test
all:
	$(CC) -c AVLtree.c 
	$(CC) AVLtest.c -lncurses AVLtree.o -o AVL_test
