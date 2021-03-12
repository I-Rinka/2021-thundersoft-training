#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <queue>
#include <sys/stat.h>
#include <sys/mman.h>
#define MM_NAME "SHARE_MM"
sem_t *Massage_2A,*Massage_2B, *Massage_2C, *Massage_2D;

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
		{
			perror("log file open failed");
			exit(-1);
		}
	};

	~LogFile()
	{
		sem_close(this->sem);
		free(this->path);
	};

	bool writeLog(const char *log)
	{
		time_t t = time(NULL);
		sem_wait(this->sem);
		char *timeStr = ctime((const time_t *)&t);
		FILE *f = fopen(this->path, "a+");

		fwrite(timeStr, sizeof(char), strlen(timeStr), f); //write date
		fwrite(log, sizeof(char), strlen(log), f);		   //write log

		fclose(f);
		sem_post(this->sem);
		return true;
	};
};

int main(int argc, const char *argv[])
{

	LogFile s = LogFile("./try.log");

	int *addr = (int *)mmap(NULL, 100, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANON, -1, 0);
	if (addr == MAP_FAILED)
	{
		perror("map address failed\n");
		exit(-1);
	}
	int *position = addr;
	int *array = addr + 1;
	*position = 0;

	int i;
	for (i = 0; i < 3; i++)
	{
		if (fork() == 0)
		{
			break;
		}
	}
	switch (i)
	{
	case 0: //进程B
	{
		char log[300] = {0};
		Massage_2C = sem_open("Massage_2C", O_CREAT, S_IRUSR | S_IWUSR, 0);
		Massage_2D = sem_open("Massage_2D", O_CREAT, S_IRUSR | S_IWUSR, 0);
		Massage_2B = sem_open("Massage_2B", O_CREAT, S_IRUSR | S_IWUSR, 0);

		s.writeLog("B start.\n");

		for (int i = 0; i < 3; i++)
		{
			sem_wait(Massage_2B);

			(*position)++;

			sem_post(Massage_2D);
			sem_post(Massage_2C);

			sprintf(log, "B Recive %d\n", *position);
			s.writeLog(log);
		}
	}
	break;
	case 1: //C
	{
		char log[300] = {0};
		Massage_2A = sem_open("Massage_2A", O_CREAT, S_IRUSR | S_IWUSR, 0);
		Massage_2C = sem_open("Massage_2C", O_CREAT, S_IRUSR | S_IWUSR, 0);

		s.writeLog("C start.\n");
		for (int i = 0; i < 3; i++)
		{
			sem_wait(Massage_2C);
 			int temp=*position;
			sprintf(&log[0], "C Recv %d\n", temp);
			s.writeLog(log);
			sleep(1);
			sem_post(Massage_2A);
		}
	}
	break;
	case 2: //D
	{
		char log[300] = {0};
		Massage_2D = sem_open("Massage_2D", O_CREAT, S_IRUSR | S_IWUSR, 0);
		s.writeLog("D start.\n");
		for (int i = 0; i < 3; i++)
		{
			sem_wait(Massage_2D);

			s.writeLog("D processing...\n");
		}
	}
	break;
	default:
	{ 
		//A
		char log[300] = {0};
		Massage_2A = sem_open("Massage_2A", O_CREAT, S_IRUSR | S_IWUSR, 0);
		Massage_2B = sem_open("Massage_2B", O_CREAT, S_IRUSR | S_IWUSR, 0);
		Massage_2C = sem_open("Massage_2C", O_CREAT, S_IRUSR | S_IWUSR, 0);
		Massage_2D = sem_open("Massage_2D", O_CREAT, S_IRUSR | S_IWUSR, 0);
		s.writeLog("A start.\n");
		int temp;
		srand(getpid());
		sem_post(Massage_2A);
		for (int i = 0; i < 3; i++)
		{
			sem_wait(Massage_2A);

			(*position)++;

			sprintf(&log[0], "A Write %d\n", *position);

			s.writeLog(log);
			sem_post(Massage_2B);
		}

		s.writeLog("A:Exit\n");
		exit(0);
	}
	break;
	}
		sem_unlink("Massage_2A");
		sem_unlink("Massage_2B");
		sem_unlink("Massage_2C");
		sem_unlink("Massage_2D");
	shm_unlink(MM_NAME);
}