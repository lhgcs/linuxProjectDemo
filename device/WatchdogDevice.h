/*
 * @Description: 看门狗
 * @Version: 1.0
 * @Autor: lhgcs
 * @Date: 2019-10-20 13:14:10
 * @LastEditors: lhgcs
 * @LastEditTime: 2019-10-29 17:27:30
 */

#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "Device.h"

class WatchdogDevice : public Device{

    // "/dev/watchdog"

    /**
     * @description: 设置参数
     * @param {type} 
     * @return: 
     */
    int setParams() {
        if(fd < 0) {
            return -1;
        }

        int timeout = 5;
        // 设置看门狗状态,开启(WDIOS_ENABLECARD)或关闭(WDIOS_DISABLECARD)
        int ret1 = ioctl(fd, WDIOC_SETOPTIONS, WDIOS_ENABLECARD);
        // 设置超时时间   
        int ret2 = ioctl(fd, WDIOC_SETTIMEOUT,&timeout);             
        if ((-1 == ret1) || (-1 == ret2)) {
           perror("WDIOC_SETTIMEOUT ioctl"); //perror函数只是将errno代表的特定的错误报告给用户
           return -1;
        }
        return 0;
    }

    /**
     * @description: 喂狗
     * @param {type} 
     * @return: 
     */
    int feedDog() {
        if(fd > 0) {
            if (-1 != ioctl(fd, WDIOC_KEEPALIVE)) {
                return 0;
            }
        }
        perror("WDIOC_KEEPALIVE ioctl");
        exit(errno);
        return -1;
    }
    
};