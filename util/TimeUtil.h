/*
 * @Description: 日期时间
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:45:42
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
    unsigned int year;  // 年
    unsigned char mon;  // 月
    unsigned char day;  // 日
    unsigned char hour; // 时
    unsigned char min;  // 分
    unsigned char sec;  // 秒
} DateTimeInfo;

class TimeUtil {

public:

    /**
    * @description: 获取当前时间
    * @param {type} 
    * @return: 
    */ 
    static int getToday(DateTimeInfo *dateTimeInfo) {
        time_t sec;          // 从1970年到现在的秒数
        struct tm *t = NULL; // 日期

        time(&sec);
        t = localtime(&sec); // 该函数将有time函数获取的值转换真实世界所使用的时间日期表示方法,localtime函数不是线程安全的,localtime_r是线程安全的
        if (NULL != t) {
            dateTimeInfo->year = t->tm_year + 1900;
            dateTimeInfo->mon = t->tm_mon + 1;
            dateTimeInfo->day = t->tm_mday;
            dateTimeInfo->hour = t->tm_hour + 12;
            dateTimeInfo->min = t->tm_min;
            dateTimeInfo->sec = t->tm_sec;
            return 0;
        }
        return -1;
    }

    /**
    * @description: 昨天日期
    * @param {type} 
    * @return: 
    */ 
    static int getYesterday(DateTimeInfo *dateTimeInfo) {
        // 获取当前日期
        if(getToday(dateTimeInfo) != 0) {
            return -1;
        }

        // 1号（需要跨月份）
        if(1 == dateTimeInfo->day) {
            switch (dateTimeInfo->mon) {
                //1月（需要跨年份）
                case 1:
                    dateTimeInfo->year -= 1;
                    dateTimeInfo->mon = 12;
                    dateTimeInfo->day = 31;
                    break;
                case 2:
                case 4:
                case 6:
                case 9:
                case 11:
                    dateTimeInfo->mon -= 1;
                    dateTimeInfo->day = 31;
                    break;
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                    dateTimeInfo->mon -= 1;
                    dateTimeInfo->day = 30;
                    break;
                case 3:
                    dateTimeInfo->mon -= 1;
                    // 闰年
                    if(((0 == dateTimeInfo->year % 4) && (0 != dateTimeInfo->year %100)) || (0 == dateTimeInfo->year %400)) {
                        dateTimeInfo->day = 29;
                    }else{
                        dateTimeInfo->day = 28;
                    }
                    break;
                default:return -1; break;
            }
        }else{
            dateTimeInfo->day -=1;
        }
        return 0;
    }

    /**
    * @description: 获取秒数
    * @param {type} 
    * @return: 
    */ 
    static long getSeconds() {
        //此函数会返回从公元1970年1月1日的UTC时间从0时0分0秒算起到现在所经过的秒数
        time_t seconds= time((time_t *)NULL);
        printf("%ld\n", seconds);

        // gmtime是把日期和时间转换为格林威治(GMT)时间的
        // asctime() asctime_r() 将时间和日期以字符串格式返回
        printf("%s\n", asctime(gmtime(&seconds)));
        printf("%s\n", ctime(&seconds));
        return seconds;
    }

    /**
    * @description: 获取毫秒数
    * @param {type} 
    * @return: 
    */ 
    static long long getMsec() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        printf("tv_sec: %ld\n", tv.tv_sec);
        printf("tv_usec: %ld\n", tv.tv_usec);
        long long ms = (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
        return ms;
    }

};