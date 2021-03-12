#ifndef _SHAREDLOGGER_HPP_
#define _SHAREDLOGGER_HPP_
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
class sharedLogger
{
public:
    sharedLogger(char *path = NULL, char *logID = (char *)"publicLogFile");
    ~sharedLogger();
    bool writeLog(const char *log, int len);

private:
    char *logID;
    char *path;
    sem_t *sem;
};

sharedLogger::sharedLogger(char *path, char *logID)
{
    this->logID = (char *)malloc(2048 * sizeof(char));
    if (sizeof(logID) < 2048 * sizeof(char))
        memcpy(this->logID, logID, strlen(logID));

    this->path = (char *)malloc(4096 * sizeof(char));
    if (path == NULL)
    {
        getcwd(this->path, 4096);
    }
    else
    {
        memcpy(this->path, path, strlen(path));
    }
    strcat(this->path, "/");
    strcat(this->path, this->logID);
    strcat(this->path, ".log");

    this->sem = sem_open(this->logID, O_CREAT, S_IRUSR | S_IWUSR, 1); //初始化信号量
    if (this->sem == NULL)
        exit(-1);
}

bool sharedLogger::writeLog(const char *log, int len)
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
    fwrite(timeStr, sizeof(char), strlen(timeStr), f) != strlen(timeStr); //写入日期
    fwrite(log, sizeof(char), len, f) != len;                             //写入日志

    fclose(f);
    sem_post(this->sem);
    return true;
}

sharedLogger::~sharedLogger()
{
    sem_close(this->sem);
    free(this->logID);
    free(this->path);
}

#endif // _SHAREDLOGGER_H_