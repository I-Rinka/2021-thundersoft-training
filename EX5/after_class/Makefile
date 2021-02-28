CC:=gcc
AR:=ar
MATH_SOURCE:=plus.c minus.c multiply.c divide.c
MATH_OBJ:=plus.o minus.o multiply.o divide.o
# MATH_OBJ:=math.o
MATH_C:=math.c

#如果只输入make，则make只会执行第一个执行树
main.out:libmath.a main.c
	$(CC) main.c -L . -l math -I . -o $@
	#编译器 源文件 库路径 库名 include位置 目标

main_dynamic.out:libmath.so
	$(CC) main.c -L . -l math -I . -o $@

libmath.so:
	$(CC) -fPIC -shared math.c -o $@
	#sudo mv ./libmath.so /usr/lib
	#如果要让动态库能跑，则最好把这个放在/usr/lib下（/lib也可以）

libmath.a:$(MATH_OBJ)
	$(AR) -rc $@ $^

$(MATH_OBJ):$(MATH_SOURCE)
	$(CC) -c $^
# $(MATH_OBJ):$(MATH_C)
# 	$(CC) -c $^

clean:
	rm *.h.gch *.o *.a *.out