#include "syslogger.h"

SysLogger* SysLogger::instance_ = NULL;

SysLogger* SysLogger::GetInstance()
{
    if (instance_ == NULL) {
        instance_ = new SysLogger();
        instance_->InitLogger("default.log",0);
    }

    return instance_;
}

SysLogger::SysLogger()
{
    min_level_ = 0;
    log_fp_ = NULL;
    log_file_ = new char[MAX_FILENAME_LEN];
    memset(log_file_, 0, MAX_FILENAME_LEN);
    log_buf_ = new char[LOG_BUF_SIZE];
    mutex_ = new pthread_mutex_t;
    pthread_mutex_init(mutex_, NULL);
}

SysLogger::~SysLogger()
{
    if (log_file_ != NULL) {
        delete[] log_file_;
        log_file_ = NULL;
    }

    if (log_buf_ != NULL) {
        delete[] log_buf_;
        log_buf_ = NULL;
    }

    if (log_fp_ != NULL) {
        fclose(log_fp_);
        log_fp_ = NULL;
    }

    pthread_mutex_destroy(mutex_);

    if (mutex_ != NULL) {
        delete mutex_;
    }
}
//open file
//set level
bool SysLogger::InitLogger(const char* file_name, int min_level)
{
    strncpy(log_file_, file_name, MAX_FILENAME_LEN - 1);

    if (min_level >=0 && min_level <= 4) {
        min_level_ = min_level;
    }

    log_fp_ = fopen(log_file_, "a");

    if (log_fp_ == NULL) {
        return false;
    }

    return true;
}
//add log data
void SysLogger::WriteLog(int level, const char* exec_file, int exec_line, int tid, const char* format, ...)
{
    if (level < min_level_) {
        return;
    }

    pthread_mutex_lock(mutex_);
    va_list valst;
    va_start(valst, format);
    set_log(level, exec_file, exec_line, tid, format, valst);
    va_end(valst);

    fputs(log_buf_, log_fp_);
    fflush(log_fp_);

    // if (log_fp_ != NULL) {
        // fclose(log_fp_);
        // log_fp_ = NULL;
    // }

    pthread_mutex_unlock(mutex_);
}

// Used for mod exit
void SysLogger::WriteLogNoLock(int level, const char* exec_file, int exec_line, int tid, const char* format, ...)
{
    if (level < min_level_) {
        return;
    }

    va_list valst;
    va_start(valst, format);
    set_log(level, exec_file, exec_line, tid, format, valst);
    va_end(valst);
    fputs(log_buf_, log_fp_);
    fflush(log_fp_);

    // if (log_fp_ != NULL) {
        // fclose(log_fp_);
        // log_fp_ = NULL;
    // }
}

void SysLogger::set_log(int level, const char* exec_file, int exec_line, int tid, const char* format, va_list valst)
{
    char exec_filename[MAX_FILENAME_LEN];
    memset(exec_filename, 0, MAX_FILENAME_LEN);
    const char* pch = strrchr(exec_file, '/');

    if (pch == NULL) {
        strncpy(exec_filename, exec_file, MAX_FILENAME_LEN - 1);
    } else {
        strncpy(exec_filename, pch + 1, MAX_FILENAME_LEN - 1);
    }

    char levstr[16];
    memset(levstr, 0, 16);

    switch (level) {
        case LEVEL_SOCKET_DEBUG:
        case LEVEL_DEBUG:
            strcpy(levstr, "DEBUG");
            break;
        case LEVEL_INFO:
            strcpy(levstr, "INFO");
            break;
        case LEVEL_WARNING:
            strcpy(levstr, "WARN");
            break;
        case LEVEL_ERROR:
            strcpy(levstr, "ERROR");
            break;
        default:
            strcpy(levstr, "INFO");
            break;
    }

    if (log_fp_ == NULL) {
        log_fp_ = fopen(log_file_, "a");
    }

    memset(log_buf_, 0, LOG_BUF_SIZE);
    struct timeval now = {0, 0};
    gettimeofday(&now, NULL);

    struct tm* sys_tm = localtime((time_t*)&(now.tv_sec));

    int n = snprintf(log_buf_, 128, "\n%d-%02d-%02d %02d:%02d:%02d,%03d <%s> [%s:%d] [%d] ",
                        sys_tm->tm_year + 1900, sys_tm->tm_mon + 1, sys_tm->tm_mday,
                        sys_tm->tm_hour, sys_tm->tm_min, sys_tm->tm_sec, now.tv_usec / 1000,
                        levstr, exec_filename, exec_line, tid);
    vsnprintf(log_buf_ + n, LOG_BUF_SIZE - n, format, valst);
}
