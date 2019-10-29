/*
 * @Description: 内存
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:43:48
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <signal.h>       /* for signal */
#include <execinfo.h>     /* for backtrace() */
#include <sys/sysinfo.h>

#define BACKTRACE_SIZE  16

#define MB_SIZE_OFFSET	1024
#define MINUTES_OFFSET	60
#define HOURS_OFFSET	(60 * MINUTES_OFFSET)
#define DAY_OFFSET		(24 * HOURS_OFFSET)

typedef struct{
    long RunDays;
    long RunHours;
    long RunMinutes;
    long UseMemorySize;
    long FreeMemorySize;
    long TotalMemorySize;
}SystemRunInfo;

class MemUtil {

public:

    /**
    * @brief getMemory 分配内存
    * @param p         指针
    * @param num       内存大小
    * @return          0：成功
    */
    static int getMemory(char **p, unsigned int num) {
        *p = (char*)malloc(sizeof(char) * num);
        if(NULL == *p) {
            return -1;
        }
        return 0;
    }

    /**
    * @description: 内存检测
    * @param {type} 
    * @return: 
    */
    static void dump(void){
        int j, nptrs;
        void *buffer[BACKTRACE_SIZE];
        char **strings;
        char buff[64];

        nptrs = backtrace(buffer, BACKTRACE_SIZE);

        memset(buff, 0, 64);
        sprintf(buff,"backtrace() returned %d addresses\n", nptrs);
        printf("%s\n", buff);

        strings = backtrace_symbols(buffer, nptrs);
        if (strings == NULL) {
            perror("backtrace_symbols");

            memset(buff, 0, 64);
            sprintf(buff,"backtrace_symbols\n");
            printf("%s\n", buff);

            exit(EXIT_FAILURE);
        }

        for (j = 0; j < nptrs; j++)  {
            memset(buff, 0, 64);
            sprintf(buff,"  [%02d] %s\n", j, strings[j]);
            printf("%s\n", buff);
        }
        free(strings);
    }

    /**
    * @description: 信号处理函数
    * @param {type} 
    * @return: 
    */
    static void signalHandler(int signo) {
        char buff[64];
        memset(buff, 0, 64);
        sprintf(buff,"\n=======>>>catch signal %d <<<=======\n",signo);
        printf("%s\n", buff);

        memset(buff, 0, 64);
        sprintf(buff,"Dump stack start...\n");
        printf("%s\n", buff);

        dump();
        memset(buff, 0, 64);
        sprintf(buff,"Dump stack end...\n");
        printf("%s\n", buff);

        //signal(SIGSEGV, signal_handler);  /* 为SIGSEGV信号安装新的处理函数 */
        signal(signo, SIG_DFL); /* 恢复信号默认处理 */
        raise(signo);           /* 重新发送信号 */
    }

    static SystemRunInfo systemRunInfo;

    static void getSystemRunInfo(SystemRunInfo *info) {
        struct sysinfo s_info;
        long RunTimeSeconds;
        sysinfo(&s_info);
        RunTimeSeconds = s_info.uptime;

        //运行时间
        info->RunDays = RunTimeSeconds / DAY_OFFSET;
        info->RunHours = (RunTimeSeconds % DAY_OFFSET) / HOURS_OFFSET;
        info->RunMinutes = (RunTimeSeconds % HOURS_OFFSET) / MINUTES_OFFSET;

        //内存信息
        info->UseMemorySize = (s_info.totalram - s_info.freeram) / MB_SIZE_OFFSET;
        info->FreeMemorySize = s_info.freeram / MB_SIZE_OFFSET;
        info->TotalMemorySize = s_info.totalram / MB_SIZE_OFFSET;
    }

//https://www.cnblogs.com/sky-heaven/p/6297675.html
//https://www.cnblogs.com/findumars/p/5927907.html
//addr2line -e backtrace 0x400a3e

};