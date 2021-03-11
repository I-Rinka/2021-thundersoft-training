# hello:hello.c hello.h
# 	gcc hello.c -o hello

OBJS1:=hello.o
OBJS2:=hello.c hello.h
CC:=gcc
hello:$(OBJS1)
	$(CC) $^ -o $@

$(OBJS1):$(OBJS2)
	$(CC) -c $< -o $@