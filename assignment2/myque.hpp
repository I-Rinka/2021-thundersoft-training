#ifndef _MYQUE_HPP_
#define _MYQUE_HPP_
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
template <typename T>
class myque
{
public:
	myque(int size)
	{
		this->maxsize = size;		
	}

	~myque()
	{

	}

	int push(T c)
	{
		if (fullFlag)
			return -1;
		data[produce] = c;
		produce = (produce + 1) % this->maxsize;
		if (consume % this->maxsize == produce)
		{
			fullFlag = true;
		}
		emptyFlag = false;
		return 0;
	}
	int pop(T *c)
	{
		if (emptyFlag)
			return -1;

		consume = (consume + 1) % this->maxsize;
		if (produce == (consume + 1) % this->maxsize)
		{
			emptyFlag = true;
		}
		fullFlag = false;
		*c = data[consume];
		return 0;
	}
	int top(T *c)
	{
		*c = data[(consume + 1) % this->maxsize];
		return 0;
	}
	bool isFull() { return fullFlag; }
	bool isEmpty() { return emptyFlag; }

	static void pass() {}


int exitflag = 0;
protected:
	T data[2];
	

private:
	int produce = 1, consume = 0, maxsize = 0;
	bool fullFlag = false, emptyFlag = true;

};
#endif // !_MYQUE_HPP_
