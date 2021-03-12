#include "myque.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include<queue>
#define MM_NAME "SHARE_MEM"

template <typename T>
class Share_Buffer
{
public:
	Share_Buffer(int size, char *bufID)
	{
		this->bufID = (char *)malloc(2048 * sizeof(char));
		if (sizeof(bufID) < 2048 * sizeof(char))
			memcpy(this->bufID, bufID, strlen(bufID));

		this->sem = sem_open(this->bufID, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1); //初始化信号量
		if (this->sem != NULL)													   //以O_EXCL创建成功，表明这是第一次，需要初始化
		{
			int fd = shm_open(this->bufID, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); //创建内存文件，注意ECXL排他标示
			// if (ftruncate(fd, sizeof(myque<T>)) == -1)
			if (ftruncate(fd, sizeof(std::queue<T>)) == -1)
				exit(-1); //重新分配大小
			// this->addr = mmap(NULL, sizeof(myque<T>), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
			this->addr = mmap(NULL, sizeof(std::queue<T>), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
			// void *tmp_ds = (char *)this->addr + sizeof(myque<T>); //数据段开始地址
			void *tmp_ds = (char *)this->addr + sizeof(std::queue<T>); //数据段开始地址
			void *tmp_dt = new (tmp_ds) T[size];
			// this->que = new (this->addr) myque<T>(size);
			this->que = new (this->addr) std::queue<T>(size);
			close(fd);
		}
		else //不是第一个程序
		{
			this->sem = sem_open(this->bufID, O_EXCL, S_IRUSR | S_IWUSR, 1); //重新打开
			struct stat shm_status;
			int fd = shm_open(this->bufID, O_RDWR, 0); //打开已有内存文件
			if (fstat(fd, &shm_status) == -1)
			{
				printf("unable to access memory");
				exit(-1);
			}
			this->addr = mmap(NULL, shm_status.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
			if (this->addr == NULL)
			{
				printf("unable to remap memory");
				exit(-1);
			}
			// this->que = (myque<T> *)this->addr;
			this->que = (std::queue<T> *)this->addr;
			close(fd);
		}
	}

	~Share_Buffer()
	{
		shm_unlink(this->bufID);
		sem_unlink(this->bufID);
		sem_close(this->sem);
		free(this->bufID);
	}
	int push(T c)
	{
		sem_wait(this->sem);
		// int tmp = que->push(c);
		this->buffer->push(c);
		sem_post(this->sem);
		return tmp;
	}
	int pop()
	{
		sem_wait(this->sem);
		// int tmp = que->pop(&d);
		this->buffer->pop();
		// *c = d;
		sem_post(this->sem);
		return 1;
	}
	int top(T *c)
	{
		sem_wait(this->sem);
		int tmp = que->top(c);
		sem_post(this->sem);
		return tmp;
	}
	bool isFull()
	{
		sem_wait(this->sem);
		bool tmp = que->isFull;
		sem_post(this->sem);
		return tmp;
	}
	bool isEmpty()
	{
		sem_wait(this->sem);
		bool tmp = que->isEmpty();
		sem_post(this->sem);
		return tmp;
	}
	bool ifExit()
	{
		return que->exitflag;
	}
	void _exit()
	{
		que->exitflag = 1;
	}

private:
	std::queue<T> *buffer;
	myque<T> *que;
	sem_t *sem;
	char *bufID;
	void *addr;
};

typedef struct point
{
	int x, y;
} P;
P p0;
sem_t *bufEmpty, *msgReceived, *toC_msgRecv, *toD_msgRecv;

class LogFile
{
private:
	char *logID;
	char *path;
	sem_t *sem;

public:
	LogFile(const char *path)
	{
		this->path = (char *)malloc(strlen(path) * sizeof(char));
		this->logID = this->path;

		memcpy(this->path, path, strlen(path));

		//share file lock
		this->sem = sem_open(basename(path), O_CREAT, S_IRUSR | S_IWUSR, 1);
		if (this->sem == NULL)
			exit(-1);
	};

	~LogFile()
	{
		sem_close(this->sem);
		free(this->logID);
		free(this->path);
	};

	bool writeLog(const char *log)
	{
		time_t t = time(NULL);
		char *timeStr = ctime((const time_t *)&t);
		sem_wait(this->sem);
		FILE *f = fopen(this->path, "a+");
		if (f == NULL)
		{
			sem_post(this->sem);
			return false;
		}
		fwrite(timeStr, sizeof(char), strlen(timeStr), f); //write date
		fwrite(log, sizeof(char), strlen(log), f);		   //write log

		fclose(f);
		sem_post(this->sem);
		return true;
	};
};

int main(int argc, const char *argv[])
{

	if (argc != 2)
		return -1;
	Share_Buffer<struct point> que = Share_Buffer<struct point>(2, (char *)MM_NAME);
	LogFile s = LogFile("./try.log");
	int i;
	for (i = 0; i < 3; i++)
	{
		if (fork() == 0)
			break;
	}
	switch (i)
	{
	case 0: //进程B
	{
		char tmp_log[200] = {0};
		bufEmpty = sem_open("bufEmpty", O_CREAT, S_IRUSR | S_IWUSR, 0);
		toC_msgRecv = sem_open("toC_msgRecv", O_CREAT, S_IRUSR | S_IWUSR, 0);
		toD_msgRecv = sem_open("toD_msgRecv", O_CREAT, S_IRUSR | S_IWUSR, 0);
		msgReceived = sem_open("msgReceived", O_CREAT, S_IRUSR | S_IWUSR, 0);

		s.writeLog("B:Sem opened.\n");
		while (true)
		{
			sem_wait(msgReceived);
			if (que.ifExit())
			{
				s.writeLog("B:exit!\n");
				sem_post(toD_msgRecv);
				sem_post(toC_msgRecv);
				exit(0);
			}
			que.top(&p0);
			sem_post(toD_msgRecv);
			sem_post(toC_msgRecv);
			sprintf(tmp_log, "B：Recv Point,\nx=%d,y=%d\n", p0.x, p0.y);
			s.writeLog(tmp_log);
		}
	}
	break;
	case 1: //进程C
	{
		char tmp_log[200] = {0};
		bufEmpty = sem_open("bufEmpty", O_CREAT, S_IRUSR | S_IWUSR, 0);
		toC_msgRecv = sem_open("toC_msgRecv", O_CREAT, S_IRUSR | S_IWUSR, 0);

		s.writeLog("C:Sem opened.\n");
		while (true)
		{
			sem_wait(toC_msgRecv);
			if (que.ifExit())
			{
				s.writeLog("C:exit!\n");
				sem_post(bufEmpty);
				exit(0);
			}
			que.top(&p0);
			sprintf(tmp_log, "C：Recv Point,\nx=%d,y=%d\n", p0.x, p0.y);
			s.writeLog(tmp_log);
			sem_post(bufEmpty);
		}
	}
	break;
	case 2: //进程D
	{
		char tmp_log[200] = {0};
		toD_msgRecv = sem_open("toD_msgRecv", O_CREAT, S_IRUSR | S_IWUSR, 0);
		s.writeLog("D:Sem opened.\n");
		while (true)
		{
			sem_wait(toD_msgRecv);
			if (que.ifExit())
			{
				s.writeLog("D:exit!\n");
				exit(0);
			}
			s.writeLog("D:processing...\n");
		}
	}
	break;

	default:
	{ //进程A
		char tmp_log[200] = {0};
		bufEmpty = sem_open("bufEmpty", O_CREAT, S_IRUSR | S_IWUSR, 0);
		sem_post(bufEmpty);
		msgReceived = sem_open("msgReceived", O_CREAT, S_IRUSR | S_IWUSR, 0);
		toC_msgRecv = sem_open("toC_msgRecv", O_CREAT, S_IRUSR | S_IWUSR, 0);
		toD_msgRecv = sem_open("toD_msgRecv", O_CREAT, S_IRUSR | S_IWUSR, 0);
		s.writeLog("A:Sem opened.\n");
		FILE *f = fopen(argv[1], "r+");

		while (fscanf(f, "%d %d ", &p0.x, &p0.y) != EOF)
		{
			sem_wait(bufEmpty);
			que.push(p0);
			sprintf(tmp_log, "A：Write Point,\nx=%d,y=%d\n", p0.x, p0.y);
			s.writeLog(tmp_log);
			sem_post(msgReceived);
		}
		s.writeLog("A:process will terminate in 5 seconds.\n");
		que._exit();
		sem_post(msgReceived);
		s.writeLog("A:Exit!\n");
		sem_unlink("bufEmpty");
		sem_unlink("msgReceived");
		sem_unlink("toC_msgRecv");
		sem_unlink("toD_msgRecv");
		exit(0);
	}
	break;
	}
}