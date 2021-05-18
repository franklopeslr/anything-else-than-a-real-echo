all: echo.c
	gcc --ansi -Wall -o echo echo.c

run: echo
	./echo