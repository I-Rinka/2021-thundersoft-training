#ifndef _LITTLEBUF_HPP_
#define _LITTLEBUF_HPP_
#include "myque.hpp"
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>



template <typename T>
class littleBuf {
public:
littleBuf(int size, char* bufID = "littleBuf_global")
{
    size = size == 0 ? 1: size;//避免空数组
    //myque(size);
    
    this->bufID = (char *)malloc(2048 * sizeof(char));
    if (sizeof(bufID) < 2048 * sizeof(char))
        memcpy(this->bufID, bufID, strlen(bufID));

    this->sem = sem_open(this->bufID, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1); //初始化信号量
    if (this->sem != NULL)//以O_EXCL创建成功，表明这是第一次，需要初始化
    {
        int fd = shm_open(this->bufID, O_RDWR | O_CREAT |O_EXCL, S_IRUSR | S_IWUSR);//创建内存文件，注意ECXL排他标示
        if(ftruncate(fd,sizeof(myque<T>)) == -1)
            exit(-1);//重新分配大小
        this->addr = mmap(NULL,sizeof(myque<T>),PROT_WRITE | PROT_READ,MAP_SHARED,fd,0);
        void* tmp_ds = (char*)this->addr + sizeof(myque<T>);//数据段开始地址
        void* tmp_dt = new(tmp_ds) T[size];
        this->q0 = new(this->addr) myque<T>(size);
        close(fd);

    }
    else//不是第一个程序
    {
        this->sem = sem_open(this->bufID, O_EXCL, S_IRUSR | S_IWUSR, 1); //重新打开
        struct stat shm_status;
        int fd = shm_open(this->bufID, O_RDWR,0);//打开已有内存文件
        if (fstat(fd, &shm_status) == -1)
            {
                printf("unable to access memory");
                exit(-1);
            }
        this->addr = mmap(NULL,shm_status.st_size,PROT_WRITE | PROT_READ,MAP_SHARED,fd,0);
        if (this->addr == NULL)
            {
                printf("unable to remap memory");
                exit(-1);
            }
        this->q0 = (myque<T>*)this->addr;
        close(fd);
    }
    
}

~littleBuf(){
    shm_unlink(this->bufID);
    sem_unlink(this->bufID);
    sem_close(this->sem);
    free(this->bufID);
}
	int push(T c)
	{
		sem_wait(this->sem);
        int tmp = q0->push(c);
        sem_post(this->sem);
        return tmp;
	}
	int pop(T *c)
	{
        sem_wait(this->sem);
        T d;
        int tmp = q0->pop(&d);
        *c = d;
        sem_post(this->sem);
        return tmp;
	}
	int top(T *c)
	{
        sem_wait(this->sem);
        int tmp = q0->top(c);
        sem_post(this->sem);
        return tmp;
	}
	bool isFull() { 
        sem_wait(this->sem);
        bool tmp =  q0->isFull; 
        sem_post(this->sem);
        return tmp;
        }
	bool isEmpty() {
        sem_wait(this->sem);
        bool tmp =  q0->isEmpty; 
        sem_post(this->sem);
        return tmp;
         }
    bool ifExit(){
        return q0->exitflag;
    }        
    void _exit(){
        q0->exitflag = 1;
    }

private:
    myque<T> *q0;
    sem_t *sem;
    char* bufID;
    void* addr;

};

#endif