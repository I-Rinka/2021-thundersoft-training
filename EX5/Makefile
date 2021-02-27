CC:=clang
AR:=ar
# MATH_SOURCE:=plus.h minus.h multiply.h divide.h
MATH_SOURCE:=plus.c minus.c multiply.c divide.c
MATH_OBJ:=plus.o minus.o multiply.o divide.o

main.out:libmath.a main.c
	$(CC) main.c -L . -lmath -I . -o $@

libmath.a:$(MATH_OBJ)
	$(AR) -rc $@ $^

$(MATH_OBJ):$(MATH_SOURCE)
	$(CC) -c $^