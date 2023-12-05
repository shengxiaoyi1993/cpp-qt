#ifndef SYSLOGGER_H_
#define SYSLOGGER_H_

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

#define LEVEL_SOCKET_DEBUG  0
#define LEVEL_DEBUG         1
#define LEVEL_INFO          2
#define LEVEL_WARNING       3
#define LEVEL_ERROR         4

#define LOG_BUF_SIZE        2048
#define MAX_FILENAME_LEN    256


using namespace std;

#define LOG_INFO_NOLOCK(format, ...) \
    SysLogger::GetInstance()->WriteLogNoLock(LEVEL_INFO, \
                                        __FILE__, __LINE__, gettid(), \
                                        format, ##__VA_ARGS__)

#define SOCKET_DEBUG(format, ...) \
    SysLogger::GetInstance()->WriteLog(LEVEL_SOCKET_DEBUG, \
                                        __FILE__, __LINE__, gettid(), \
                                        format, ##__VA_ARGS__)

#define LOG_DEBUG(format, ...) \
    SysLogger::GetInstance()->WriteLog(LEVEL_DEBUG, \
                                        __FILE__, __LINE__, gettid(), \
                                        format, ##__VA_ARGS__)

#define LOG_INFO(format, ...) \
    SysLogger::GetInstance()->WriteLog(LEVEL_INFO, \
                                        __FILE__, __LINE__, gettid(), \
                                        format, ##__VA_ARGS__)

#define LOG_WARNING(format, ...) \
    SysLogger::GetInstance()->WriteLog(LEVEL_WARNING, \
                                        __FILE__, __LINE__, gettid(), \
                                        format, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
    SysLogger::GetInstance()->WriteLog(LEVEL_ERROR, \
                                        __FILE__, __LINE__, gettid(), \
                                        format, ##__VA_ARGS__)

// Pthread trylock, lock, unlock
#define MUTEX_LOCK(mutex) {int oldstate;\
            pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);\
            pthread_mutex_lock(mutex);\
            pthread_setcancelstate(oldstate, NULL);\
}

#define MUTEX_UNLOCK(mutex) {int oldstate;\
            pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);\
            pthread_mutex_unlock(mutex);\
            pthread_setcancelstate(oldstate, NULL);\
}

inline int MutexTryLock(pthread_mutex_t* mutex)
{
    int ret = 0;
    int oldstate;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
    ret = pthread_mutex_trylock(mutex);
    pthread_setcancelstate(oldstate, NULL);
    return ret;
}

inline void MutexLock(pthread_mutex_t* mutex)
{
    int oldstate;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
    pthread_mutex_lock(mutex);
    pthread_setcancelstate(oldstate, NULL);
}

inline void MutexUnlock(pthread_mutex_t* mutex)
{
    int oldstate;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
    pthread_mutex_unlock(mutex);
    pthread_setcancelstate(oldstate, NULL);
}

class SysLogger
{
public:
    SysLogger();
    ~SysLogger();

    static SysLogger* GetInstance();

    bool InitLogger(const char* file_name, int min_level);
    void WriteLog(int level, const char* exec_file, int exec_line, int tid, const char* format, ...);
    void WriteLogNoLock(int level, const char* exec_file, int exec_line, int tid, const char* format, ...);

private:
    void set_log(int level, const char* exec_file, int exec_line, int tid, const char* format, va_list valst);

public:
    static SysLogger*   instance_;

private:
    int     min_level_;
    char*   log_file_;
    char*   log_buf_;
    FILE*   log_fp_;
    pthread_mutex_t*    mutex_;
};

#endif
